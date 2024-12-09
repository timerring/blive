# Copyright (c) 2024 bilive.

import subprocess
import os
import sys
from src import allconfig
from datetime import datetime
from src.upload.generate_yaml import generate_yaml_template
from src.upload.extract_video_info import generate_title
import time

def read_and_delete_lines(file_path):
    while True:
        if os.path.getsize(file_path) == 0:
            print(f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} Empty queue, wait 2 minutes and check again...", flush=True)
            time.sleep(120)
            continue

        with open(file_path, "r") as file:
            lines = file.readlines()
            upload_video_path = lines.pop(0).strip()
            print(f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} " + "deal with " + upload_video_path, flush=True)
            # generate the yaml template
            yaml_template = generate_yaml_template(upload_video_path)
            yaml_file_path = allconfig.SRC_DIR + "/upload/upload.yaml"
            with open(yaml_file_path, 'w', encoding='utf-8') as file:
                file.write(yaml_template)
            upload_video(upload_video_path, yaml_file_path)
        with open(file_path, "w") as file:
            file.writelines(lines)

def upload_video(upload_path, yaml_file_path):
    try:
        # Construct the command
        command = [
            f"{allconfig.BILIVE_DIR}/src/upload/biliup",
            "-u",
            f"{allconfig.SRC_DIR}/upload/cookies.json",
            "upload",
            upload_path,
            "--config",
            yaml_file_path
        ]
        
        # Execute the command
        result = subprocess.run(command, check=True)
        
        # Check if the command was successful
        if result.returncode == 0:
            print("Upload successfully, then delete the video")
            os.remove(upload_path)
        else:
            print("Fail to upload, the files will be reserved.")
            sys.exit(1)
    
    except subprocess.CalledProcessError:
        print("Fail to upload, the files will be reserved.")
        sys.exit(1)

def find_bv_number(target_str, my_list):
    for element in my_list:
        if target_str in element:
            parts = element.split('\t')
            if len(parts) > 0:
                return parts[0]
    return None

def read_append_and_delete_lines(file_path):
    while True:
        if os.path.getsize(file_path) == 0:
            print(f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} Empty queue, wait 2 minutes and check again...", flush=True)
            time.sleep(120)
            continue

        with open(file_path, "r") as file:
            lines = file.readlines()
            upload_video_path = lines.pop(0).strip()
            print(f"{datetime.now().strftime('%Y-%m-%d %H:%M:%S')} " + "deal with " + upload_video_path, flush=True)
            # check if the live is already uploaded
            query = generate_title(upload_video_path)
            result = subprocess.check_output(f"{allconfig.SRC_DIR}/upload/biliup" + " -u " + f"{allconfig.SRC_DIR}/upload/cookies.json" + " list", shell=True)
            upload_list = result.decode("utf-8").splitlines()
            limit_list = upload_list[:30]
            bv_result = find_bv_number(query, limit_list)
            if bv_result:
                print(f"BV number is: {bv_result}", flush=True)
                append_upload(upload_video_path, bv_result)
            else:
                print("First upload this live", flush=True)
                # generate the yaml template
                yaml_template = generate_yaml_template(upload_video_path)
                yaml_file_path = allconfig.SRC_DIR + "/upload/upload.yaml"
                with open(yaml_file_path, 'w', encoding='utf-8') as file:
                    file.write(yaml_template)
                upload_video(upload_video_path, yaml_file_path)
            with open(file_path, "w") as file:
                file.writelines(lines)

def append_upload(upload_path, bv_result):
    try:
        # Construct the command
        command = [
            f"{allconfig.BILIVE_DIR}/src/upload/biliup",
            "-u",
            f"{allconfig.SRC_DIR}/upload/cookies.json",
            "append",
            "--vid",
            bv_result,
            upload_path
        ]
        # Execute the command
        result = subprocess.run(command, check=True)
        
        # Check if the command was successful
        if result.returncode == 0:
            print("Upload successfully, then delete the video")
            os.remove(upload_path)
        else:
            print("Fail to upload, the files will be reserved.")
            sys.exit(1)
    
    except subprocess.CalledProcessError:
        print("Fail to upload, the files will be reserved.")
        sys.exit(1)

if __name__ == "__main__":    
    # read the queue and upload the video
    queue_path = allconfig.SRC_DIR + "/upload/uploadVideoQueue.txt"
    # read_and_delete_lines(queue_path)
    read_append_and_delete_lines(queue_path)