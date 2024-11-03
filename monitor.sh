#!/bin/bash
# DESCRIPTION:
#   The script mainly monitor the video folders and execute the recorded videos.
#
# PARAMETERS:
#   INPUT: 
#       $1 - the video folder (default: $root_path/Videos)
#   OUTPUT: none.

while read key value; do
    export $key="$value"
done < ./path.txt

if [ $# -gt 0 ]; then
    target_folder="$1"
else
    target_folder="$root_path/Videos"
fi


function monitor_directory() {
    for file in "$1"/*; do
        if [ -d "$file" ]; then
            monitor_directory "$file"
        elif [[ "$file" == *.mp4 ]]; then
            $root_path/danmakuBurning.sh $file
        fi
    done
}

while true; do
    monitor_directory "$target_folder"
    sleep 180
done