# -*- coding: UTF-8 -*-

import io
import multiprocessing
import subprocess
import tempfile
import time
import warnings
warnings.filterwarnings('ignore')
import librosa
import os
import stat
import audioop
import wave
import math
import sys
import pysrt
import six
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import config
import whisper
from zhconv.zhconv import convert
import argparse

def process_line(line):
    # Due to the base model's poor performance, we need to remove the lines containing below words:
    if "谢谢" in line:
        return None
    
    if "字幕" in line:
        return None

    if "by" in line:
        return None

    # Truncate lines longer than 40 characters
    if len(line) > 40:
        line = line[:40]

    # Remove characters that repeat more than 6 times consecutively
    import re
    line = re.sub(r'(.)\1{6,}', r'\1', line)

    return line

def process_srt_file(input_file):
    # Read the contents of the file first
    with open(input_file, 'r', encoding='utf-8') as infile:
        lines = infile.readlines()

    # Open the file again in write mode to overwrite it
    with open(input_file, 'w', encoding='utf-8') as outfile:
        for line in lines:
            # Process only lines that are not timestamps or empty
            if not line.strip().isdigit() and '-->' not in line and line.strip():
                processed_line = process_line(line.strip())
                if processed_line is not None:
                    outfile.write(processed_line + '\n')
            else:
                # Write timestamps and other non-text lines unchanged
                outfile.write(line)

class AudioRecogniser:
    def __init__(self, language='auto'):
        self.model_path = config.get_model_path()
        self.model = whisper.load_model(self.model_path)
        self.language = language

    def __call__(self, audio_data):
        audio_data = whisper.pad_or_trim(audio_data)
        mel = whisper.log_mel_spectrogram(audio_data).to(self.model.device)

        # detect audio language
        _, probs = self.model.detect_language(mel)

        # decode the audio
        if self.language != 'auto':
            if self.language in ('zh-cn', 'zh-tw', 'zh-hk', 'zh-sg', 'zh-hans', 'zh-hant'):
                options = whisper.DecodingOptions(fp16=False, language='zh')
            else:
                options = whisper.DecodingOptions(fp16=False, language=self.language)
        else:
            # if no language is set, detect language automatically
            print(f"{config.get_interface_config()['Main']['LanguageDetected']}{max(probs, key=probs.get)}")
            options = whisper.DecodingOptions(fp16=False)

        transcription = whisper.decode(self.model, mel, options)
        zh_list = ('zh', 'zh-cn', 'zh-tw', 'zh-hk', 'zh-sg', 'zh-hans', 'zh-hant')
        if self.language in zh_list:
            text = convert(transcription.text, self.language)
        elif max(probs, key=probs.get) in zh_list:
            text = convert(transcription.text, 'zh-cn')
        else:
            text = transcription.text
        return text


class FLACConverter:  # pylint: disable=too-few-public-methods
    """
    Class for converting a region of an input audio or video file into a FLAC audio file
    """

    def __init__(self, source_path, include_before=0.25, include_after=0.25):
        self.source_path = source_path
        self.include_before = include_before
        self.include_after = include_after

    def __call__(self, region):
        try:
            start, end = region
            start = max(0, start - self.include_before)
            end += self.include_after
            temp = tempfile.NamedTemporaryFile(suffix='.flac', delete=False)
            command = ["ffmpeg", "-ss", str(start), "-t", str(end - start),
                       "-y", "-i", self.source_path,
                       "-loglevel", "error", temp.name]
            use_shell = True if os.name == "nt" else False
            subprocess.check_output(command, stdin=open(os.devnull), shell=use_shell)
            read_data = temp.read()
            temp.close()
            os.unlink(temp.name)
            return read_data

        except KeyboardInterrupt:
            return None


class SubtitleGenerator:
    # if you want to change the recognition language, you can change the zh-cn to others, such as en
    def __init__(self, filename, language='zh-cn'):
        self.filename = filename
        self.language = language
        self.isFinished = False

    @staticmethod
    def which(program):
        """
        Return the path for a given executable.
        """

        def is_exe(file_path):
            """
            Checks whether a file is executable.
            """
            if not os.access(file_path, os.X_OK):
                os.chmod(file_path, stat.S_IXUSR)
            if not os.access(file_path, os.X_OK):
                os.chmod(file_path, stat.S_IXGRP)
            if not os.access(file_path, os.X_OK):
                os.chmod(file_path, stat.S_IXOTH)
            return os.path.isfile(file_path) and os.access(file_path, os.X_OK)

        fpath, _ = os.path.split(program)
        if fpath:
            if is_exe(program):
                return program
        else:
            for path in os.environ["PATH"].split(os.pathsep):
                path = path.strip('"')
                exe_file = os.path.join(path, program)
                if is_exe(exe_file):
                    return exe_file
        return None

    def extract_audio(self, rate=16000):
        """
        Extract audio from an input file to a temporary WAV file.
        """
        temp = tempfile.NamedTemporaryFile(suffix='.wav', delete=False)
        if not os.path.isfile(self.filename):
            print("The given file does not exist: {}".format(self.filename))
            raise Exception("Invalid filepath: {}".format(self.filename))
        command = ["ffmpeg", "-y", "-i", self.filename,
                   "-ac", '1', "-ar", str(rate),
                   "-loglevel", "error", temp.name]
        use_shell = True if os.name == "nt" else False
        subprocess.check_output(command, stdin=open(os.devnull), shell=use_shell)
        return temp.name, rate

    @staticmethod
    def percentile(arr, percent):
        """
        Calculate the given percentile of arr.
        """
        arr = sorted(arr)
        index = (len(arr) - 1) * percent
        floor = math.floor(index)
        ceil = math.ceil(index)
        if floor == ceil:
            return arr[int(index)]
        low_value = arr[int(floor)] * (ceil - index)
        high_value = arr[int(ceil)] * (index - floor)
        return low_value + high_value

    def find_speech_regions(self, filename, frame_width=4096, min_region_size=0.5,
                            max_region_size=6):  # pylint: disable=too-many-locals
        """
        Perform voice activity detection on a given audio file.
        """
        reader = wave.open(filename)
        sample_width = reader.getsampwidth()
        rate = reader.getframerate()
        n_channels = reader.getnchannels()
        chunk_duration = float(frame_width) / rate

        n_chunks = int(math.ceil(reader.getnframes() * 1.0 / frame_width))
        energies = []
        for _ in range(n_chunks):
            chunk = reader.readframes(frame_width)
            energies.append(audioop.rms(chunk, sample_width * n_channels))
        threshold = self.percentile(energies, 0.2)
        elapsed_time = 0

        regions = []
        region_start = None

        for energy in energies:
            is_silence = energy <= threshold
            max_exceeded = region_start and elapsed_time - region_start >= max_region_size

            if (max_exceeded or is_silence) and region_start:
                if elapsed_time - region_start >= min_region_size:
                    regions.append((region_start, elapsed_time))
                    region_start = None

            elif (not region_start) and (not is_silence):
                region_start = elapsed_time
            elapsed_time += chunk_duration
        return regions

    @staticmethod
    def srt_formatter(subtitles, padding_before=0, padding_after=0):
        """
        Serialize a list of subtitles according to the SRT format, with optional time padding.
        """
        sub_rip_file = pysrt.SubRipFile()
        for i, ((start, end), text) in enumerate(subtitles, start=1):
            item = pysrt.SubRipItem()
            item.index = i
            item.text = six.text_type(text)
            item.start.seconds = max(0, start - padding_before)
            item.end.seconds = end + padding_after
            sub_rip_file.append(item)
        return '\n'.join(six.text_type(item) for item in sub_rip_file)

    def run(self, output=None):
        """
        Given an input audio/video file, generate subtitles in the specified language and format.
        """
        audio_filename, audio_rate = self.extract_audio()
        regions = self.find_speech_regions(audio_filename)
        pool = multiprocessing.Pool(12)
        converter = FLACConverter(source_path=audio_filename)
        recognizer = AudioRecogniser(language=self.language)
        transcripts = []
        print(f"{config.get_interface_config()['Main']['StartGenerateSub']}")
        start_time = time.time()

        if regions:
            try:
                extracted_regions = []
                for i, extracted_region in enumerate(pool.imap(converter, regions)):
                    data, sr = librosa.load(io.BytesIO(extracted_region), sr=16000)
                    extracted_regions.append(data)

                for i, data in enumerate(extracted_regions):
                    transcript = recognizer(data)
                    print(transcript)
                    transcripts.append(transcript)
                    print()

            except KeyboardInterrupt:
                pool.terminate()
                pool.join()
                print("Cancelling transcription")
                raise

        timed_subtitles = [(r, t) for r, t in zip(regions, transcripts) if t]

        formatted_subtitles = self.srt_formatter(subtitles=timed_subtitles)
        dest = output
        if not dest:
            base = os.path.splitext(self.filename)[0]
            dest = "{base}.{format}".format(base=base, format="srt")

        with open(dest, 'wb') as output_file:
            output_file.write(formatted_subtitles.encode("utf-8"))
        process_srt_file(dest)
        os.remove(audio_filename)
        self.isFinished = True
        elapse = time.time() - start_time
        print(f"{config.get_interface_config()['Main']['FinishGenerateSub']}")
        print(f"{config.get_interface_config()['Main']['SubLocation']}{dest}")
        print(f"{config.get_interface_config()['Main']['Elapse']}: {elapse}s")
        return dest


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Subtitle Generator')
    
    parser.add_argument('-l', '--language', help=config.get_interface_config()['LanguageModeGUI']['SubtitleLanguage'], required=False)
    parser.add_argument('filename', nargs='?', help=config.get_interface_config()['Main']['InputFile'])

    args = parser.parse_args()

    if hasattr(args, 'help'):
        exit()

    video_path = args.filename or input(f"{config.get_interface_config()['Main']['InputFile']}").strip()
    sg = SubtitleGenerator(video_path, language='zh-cn')
    print('Start project.')
    sg.run()