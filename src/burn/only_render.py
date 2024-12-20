# Copyright (c) 2024 bilive.

import argparse
import os
import subprocess
from src.config import GPU_EXIST, SRC_DIR, MODEL_TYPE
from src.burn.generate_danmakus import get_resolution, process_danmakus
from src.burn.generate_subtitles import generate_subtitles
from src.burn.render_video import render_video
import queue
import threading
import time

def normalize_video_path(filepath):
    """Normalize the video path to upload
    Args:
        filepath: str, the path of video
    """
    parts = filepath.rsplit('/', 1)[-1].split('_')
    date_time_parts = parts[1].split('-')
    new_date_time = f"{date_time_parts[0][:4]}-{date_time_parts[0][4:6]}-{date_time_parts[0][6:8]}-{date_time_parts[1]}-{date_time_parts[2]}"
    return filepath.rsplit('/', 1)[0] + '/' + parts[0] + '_' + new_date_time + '-.mp4'

def render_video_only(video_path):
    if not os.path.exists(video_path):
        print(f"File {video_path} does not exist.")
        return

    original_video_path = str(video_path)
    format_video_path = normalize_video_path(original_video_path)
    xml_path = original_video_path[:-4] + '.xml'
    ass_path = original_video_path[:-4] + '.ass'
    srt_path = original_video_path[:-4] + '.srt'
    jsonl_path = original_video_path[:-4] + '.jsonl'

    # Recoginze the resolution of video
    video_resolution = get_resolution(original_video_path)
    try:
        # Process the danmakus to ass and remove emojis
        subtitle_font_size, subtitle_margin_v = process_danmakus(xml_path, video_resolution)
    except TypeError as e:
        print(f"TypeError: {e} - Check the return value of process_danmakus")
    except FileNotFoundError as e:
        print(f"FileNotFoundError: {e} - Check if the file exists")

    # Generate the srt file via whisper model
    if GPU_EXIST:
        if MODEL_TYPE != "pipeline":
            generate_subtitles(original_video_path)

    # Burn danmaku or subtitles into the videos 
    render_video(original_video_path, format_video_path, subtitle_font_size, subtitle_margin_v)
    print("complete danamku burning and wait for uploading!", flush=True)

    # # Delete relative files
    for remove_path in [original_video_path, xml_path, ass_path, srt_path, jsonl_path]:
        if os.path.exists(remove_path):
            os.remove(remove_path)
    
    # # For test
    # test_path = original_video_path[:-4]
    # os.rename(original_video_path, test_path)

    with open(f"{SRC_DIR}/upload/uploadVideoQueue.txt", "a") as file:
        file.write(f"{format_video_path}\n")

class VideoRenderQueue:
    def __init__(self):
        self.render_queue = queue.Queue()

    def pipeline_render(self, video_path):
        generate_subtitles(video_path)
        self.render_queue.put(video_path)

    def monitor_queue(self):
        while True:
            if not self.render_queue.empty():
                video_path = self.render_queue.get()
                try:
                    render_video_only(video_path)
                except Exception as e:
                    print(f"Error processing video {video_path}: {e}", flush=True)
            else:
                time.sleep(1)

if __name__ == '__main__':
    # Read and define variables
    parser = argparse.ArgumentParser(description='Danmaku burns')
    parser.add_argument('video_path', type=str, help='Path to the Video file')
    args = parser.parse_args()
    original_video_path = args.video_path
    render_video_only(original_video_path)