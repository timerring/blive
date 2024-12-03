# Copyright (c) 2024 bilive.

import subprocess
import src.allconfig
import os

# Generate the srt file via whisper model
def generate_subtitles(in_video_path):
    """Generate subtitles via whisper model
    Args:
        in_video_path: str, the path of video
    """
    with open(src.allconfig.WHISPER_LOG_PATH, 'a') as wlog:
        subprocess.run(['python', os.path.join(src.allconfig.SRC_DIR, 'subtitle', 'generate.py'), in_video_path], stdout=wlog)