import subprocess
import src.allconfig
import os

# Generate the srt file via whisper model
def generate_subtitles(in_video_path):
    with open(src.allconfig.WHISPER_LOG_PATH, 'a') as wlog:
        subprocess.run(['python', os.path.join(src.allconfig.SRC_DIR, 'subtitle', 'generate.py'), in_video_path], stdout=wlog)