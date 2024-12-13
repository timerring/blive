# Copyright (c) 2024 bilive.

import subprocess
import src.allconfig
import os

def render_video(in_video_path, out_video_path, in_subtitle_font_size, in_subtitle_margin_v):
    """Burn the danmakus and subtitles into the videos
    Args:
        in_video_path: str, the path of video
        out_video_path: str, the path of rendered video
        in_subtitle_font_size: str, the font size of subtitles
        in_subtitle_margin_v: str, the bottom margin of subtitles
    """
    in_ass_path = in_video_path[:-4] + '.ass'
    if src.allconfig.GPU_EXIST:
        in_srt_path = in_video_path[:-4] + '.srt'
        if os.path.isfile(in_ass_path):
            print("wisper danmaku")
            command = [
                'ffmpeg', '-y', '-hwaccel', 'cuda', '-c:v', 'h264_cuvid', '-i', in_video_path,
                '-c:v', 'h264_nvenc', '-vf', f"subtitles={in_srt_path}:force_style='Fontsize={in_subtitle_font_size},MarginV={in_subtitle_margin_v}',subtitles={in_ass_path}", out_video_path
            ]
            with open(src.allconfig.BURN_LOG_PATH, 'a') as blog:
                subprocess.run(command, stdout=blog, stderr=subprocess.STDOUT)
        else:
            print("wisper no danmaku")
            command_no_danmaku = [
                'ffmpeg', '-y', '-hwaccel', 'cuda', '-c:v', 'h264_cuvid', '-i', in_video_path,
                '-c:v', 'h264_nvenc', '-vf', f"subtitles={in_srt_path}:force_style='Fontsize={in_subtitle_font_size},MarginV={in_subtitle_margin_v}'", out_video_path
            ]
            with open(src.allconfig.BURN_LOG_PATH, 'a') as blog:
                subprocess.run(command_no_danmaku, stdout=blog, stderr=subprocess.STDOUT)
    else:
        if os.path.isfile(in_ass_path):
            print("no gpu danmaku")
            command_without_gpu = [
                'ffmpeg', '-y', '-i', in_video_path, '-vf', f'ass={in_ass_path}', '-preset', 'ultrafast', out_video_path
            ]
            with open(src.allconfig.BURN_LOG_PATH, 'a') as blog:
                subprocess.run(command_without_gpu, stdout=blog, stderr=subprocess.STDOUT)
        else:
            print("no gpu no danmaku")
            subprocess.run(['mv', in_video_path, out_video_path])