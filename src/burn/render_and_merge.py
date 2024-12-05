# Copyright (c) 2024 bilive.

import argparse
import os
import subprocess
from src.allconfig import GPU_EXIST, SRC_DIR, MERGE_LOG_PATH
from src.burn.generate_danmakus import get_resolution, process_danmakus
from src.burn.generate_subtitles import generate_subtitles
from src.burn.render_video import render_video
from src.upload.extract_video_info import get_video_info

def normalize_video_path(filepath):
    """Normalize the video path to upload
    Args:
        filepath: str, the path of video
    """
    parts = filepath.rsplit('/', 1)[-1].split('_')
    date_time_parts = parts[1].split('-')
    new_date_time = f"{date_time_parts[0][:4]}-{date_time_parts[0][4:6]}-{date_time_parts[0][6:8]}-{date_time_parts[1]}"
    return filepath.rsplit('/', 1)[0] + '/' + parts[0] + '_' + new_date_time + '-upload.mp4'

def merge_videos(in_final_video, title, artist, date, merge_list):
    """Merge the video segments and preserve the first video's metadata
    Args:
        in_final_video: str, the path of videos will be merged
    """
    merge_command = [
    'ffmpeg', '-f', 'concat', '-safe', '0', '-i', merge_list, '-metadata', f'title={title}', '-metadata', f'artist={artist}', '-metadata', f'date={date}', '-use_wallclock_as_timestamps', '1', 
    '-c', 'copy', in_final_video
    ]
    with open(MERGE_LOG_PATH, 'a') as mlog:
        subprocess.run(merge_command, stdout=mlog, stderr=subprocess.STDOUT)
    subprocess.run(['rm', merge_list])

def render_and_merge(video_path_list):
    title = ''
    artist = ''
    date = ''
    output_video_path = ''
    merge_list = SRC_DIR + '/burn/mergevideo.txt'

    for line in video_path_list:
        stripped_line = str(line).strip()
        if stripped_line:
            directory = os.path.dirname(stripped_line)
            video_name = os.path.basename(stripped_line)
            tmp = directory + '/tmp/'
            if output_video_path == '':
                title, artist, date = get_video_info(stripped_line)
                output_video_path = normalize_video_path(stripped_line)
                print("The output video is " + output_video_path)
                subprocess.run(['mkdir', tmp])
            
            video_to_be_merged = tmp + video_name
            original_video_path = stripped_line
            xml_path = original_video_path[:-4] + '.xml'
            ass_path = original_video_path[:-4] + '.ass'
            srt_path = original_video_path[:-4] + '.srt'
            jsonl_path = original_video_path[:-4] + '.jsonl'
            # Recoginze the resolution of video
            video_resolution = get_resolution(original_video_path)
            # Process the danmakus to ass and remove emojis
            subtitle_font_size = process_danmakus(xml_path, video_resolution)
            # Generate the srt file via whisper model
            if GPU_EXIST:
                generate_subtitles(original_video_path)
            # Burn danmaku or subtitles into the videos 
            render_video(original_video_path, video_to_be_merged, subtitle_font_size)
            if not os.path.exists(merge_list):
                open(merge_list, 'w').close() 
            with open(merge_list, 'a') as f:
                f.write(f"file '{video_to_be_merged}'\n")
            print("complete danamku burning and wait for uploading!", flush=True)
    
        # for remove_path in [original_video_path, xml_path, ass_path, srt_path, jsonl_path]:
        #     if os.path.exists(remove_path):
        #         os.remove(remove_path)
        
        # For test part
        test_path = original_video_path[:-4]
        os.rename(original_video_path, test_path)
        
    merge_videos(output_video_path, title, artist, date, merge_list)
    subprocess.run(['rm', '-r', tmp])

    with open(f"{SRC_DIR}/upload/uploadVideoQueue.txt", "a") as file:
        file.write(f"{output_video_path}\n")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Danmaku burns')
    parser.add_argument('video_path', type=str, help='Path to the Video file')
    args = parser.parse_args()
    video_path = args.video_path
    render_and_merge(video_path)