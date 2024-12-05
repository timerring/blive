# Copyright (c) 2024 bilive.

import os
from pathlib import Path
from src.burn.only_render import render_video_only
from src.burn.render_and_merge import render_and_merge
import time
from src.allconfig import VIDEOS_DIR

def process_folder(folder_path):
    # Don't process the recording folder
    flv_files = list(Path(folder_path).glob('*.flv'))
    if flv_files:
        print(f"Found flv files in {folder_path}. Skipping.")
        return

    files_by_date = {}

    # process the recorded files
    mp4_files = [mp4_file for mp4_file in Path(folder_path).glob('*.mp4') if not mp4_file.name.endswith('-upload.mp4')]
    for mp4_file in mp4_files:
        date_part = mp4_file.stem.split('_')[1].split('-')[0]

        if date_part not in files_by_date:
            files_by_date[date_part] = []
        files_by_date[date_part].append(mp4_file)

    for date, files in files_by_date.items():
        if len(files) > 1:
            # If there are multiple segments with the same date, merge them
            sorted_files = sorted(files, key=lambda x: x.stem.split('_')[1])
            render_and_merge(sorted_files)
        else:
            for file in files:
                render_video_only(file)

if __name__ == "__main__":
    room_folder_path = VIDEOS_DIR
    while True:
        for room_folder in Path(room_folder_path).iterdir():
            if room_folder.is_dir():
                process_folder(room_folder)
        print(f"{time.strftime('%Y-%m-%d %H:%M:%S')} There is no file recorded. Check again in 120 seconds.", flush=True)
        time.sleep(120)