# Copyright (c) 2024 bilive.

import subprocess
from src.utils.adjustPrice import update_sc_prices
from src.config import DanmakuFactory_PATH
import os
from src.utils.removeEmojis import *

def get_resolution(in_video_path):
    """Return the resolution of video
    Args:
        in_video_path: str, the path of video
    Return:
        resolution: str.
    """
    try:
        # Use ffprobe to acquire the video resolution
        result = subprocess.run(
            ['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=width,height', '-of', 'csv=s=x:p=0', in_video_path],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            check=True
        )
        resolution = result.stdout.strip()
        print("The video resolution is " + resolution, flush=True)
        return resolution
    except subprocess.CalledProcessError as e:
        print(f"Error: {e.stderr}")
        return None

def process_danmakus(in_xml_path, resolution):
    """Generate and process the danmakus according to different resolution.
    Args:
        in_xml_path: str, the xml path to generate ass file
        resolution: str, the resolution of the video
    Return:
        subtitle_font_size: str, the font size of subtitles
    """
    if os.path.isfile(in_xml_path):
        # Adjust the price of sc and guard
        update_sc_prices(in_xml_path)
        in_ass_path = in_xml_path[:-4] + '.ass'
        if resolution == '1280x720':
            boxsize = '470x720'
            boxfont = '25'
            danmakufont = '38'
            subtitle_font_size = '15'
            subtitle_margin_v = '40'
        elif resolution == '1920x1080':
            boxsize = '500x1080'
            boxfont = '50'
            danmakufont = '55'
            subtitle_font_size = '16'
            subtitle_margin_v = '60'
        elif resolution == '1080x1920':
            boxsize = '500x1920'
            boxfont = '55'
            danmakufont = '60'
            subtitle_font_size = '8'
            subtitle_margin_v = '60'
        elif resolution == '720x1280':
            boxsize = '500x1280'
            boxfont = '28'
            danmakufont = '38'
            subtitle_font_size = '8'
            subtitle_margin_v = '60'
        else:
            boxsize = '500x1080'
            boxfont = '38'
            danmakufont = '38'
            subtitle_font_size = '16'
            subtitle_margin_v = '60'
        # Convert danmakus to ass file
        subprocess.run([DanmakuFactory_PATH, "-o", in_ass_path, "-i", in_xml_path, "--resolution", resolution, "--msgboxsize", boxsize, "--msgboxfontsize", boxfont, "-S", danmakufont, "--ignore-warnings"])
        # Remove emojis from ass danmakus (the ffmpeg do not support emojis)
        remove_emojis(in_ass_path)
        print(f"The {in_ass_path} has been processed.", flush=True)
        return subtitle_font_size, subtitle_margin_v