#!/bin/bash

if [ $# -gt 0 ]; then
    target_folder="$1"
else
    target_folder="/root/blive/Videos"
fi

queue=("$target_folder")

while [ ${#queue[@]} -gt 0 ]; do
    current_folder=${queue[0]}
    unset queue[0]
    queue=( "${queue[@]}" )

    for file in "$current_folder"/*; do
        if [ -d "$file" ]; then
            queue=( "${queue[@]}" "$file" )
        elif [[ $file == *.mp4 ]]; then
            xml_file="${file%.mp4}.xml"
            if [ -f "$xml_file" ]; then
	       absolute_mp4_path=$(readlink -f "$file")
	       echo "$absolute_mp4_path"
               /root/blive/biliup/formatName.sh "$absolute_mp4_path"
            fi
        fi
    done
done