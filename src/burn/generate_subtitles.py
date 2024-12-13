# Copyright (c) 2024 bilive.

import subprocess
from src.config import WHISPER_LOG_PATH, SRC_DIR
import os

# Generate the srt file via whisper model
def generate_subtitles(in_video_path):
    """Generate subtitles via whisper model
    Args:
        in_video_path: str, the path of video
    """
    with open(WHISPER_LOG_PATH, 'a') as wlog:
        subprocess.run(['python', os.path.join(SRC_DIR, 'subtitle', 'generate.py'), in_video_path], stdout=wlog)