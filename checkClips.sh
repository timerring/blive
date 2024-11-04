#!/bin/bash
# DESCRIPTION:
#   Check the recorded folders in loop, if there are the same date videos, handle them in burnAndMerge script. 
#   Otherwise, handle them in danmakuBurning script.
#
# PARAMETERS:
#   INPUT:  none.
#   OUTPUT: $same_date_videos > sameVideos.txt

while read key value; do
    export $key="$value"
done < ./path.txt
echo $root_path

check_and_process_folder() {
    local folder_path=$1
    local date_part
    local count=0
    local same_date_videos=""

    # Check if there are any flv files in the folder.
    # If exsits, which means the room is under recording.
    flv_file=("$folder_path"/*.flv)
    flv_name=$(basename "$flv_file")
    if [ ${#flv_name} -gt 10 ]; then
        echo "Found flv files in $folder_path. Skipping."
        return
    fi

    # Check if there is the video clips in the same date
    for mp4_file in "$folder_path"/*.mp4; do
        detect_name=$(basename "$mp4_file")
        # This length to prevent processing the burned videos again!
        if [ ${#detect_name} -gt 27 ]; then
            date_part=$(basename "$mp4_file" | cut -d '-' -f 1)
            for other_mp4_file in "$folder_path"/*.mp4; do
                if [[ $(basename "$other_mp4_file" | cut -d '-' -f 1) == $date_part ]]; then
                    count=$((count + 1))
                fi
            done
            if [ $count -gt 1 ]; then
                same_date_videos="$same_date_videos\n$mp4_file"
            fi
        fi
    done

    # If the same date videos exist then merge them
    if [ -n "$same_date_videos" ]; then
        echo -e "$same_date_videos" > sameVideos.txt
        $root_path/burningAndMerge.sh sameVideos.txt
    else
    # Else upload the single video via formatname
        for mp4_file in "$folder_path"/*.mp4; do
        detect_name=$(basename "$mp4_file")
        # This length to prevent processing the burned videos again!
        if [ ${#detect_name} -gt 27 ]; then
            $root_path/danmakuBurning.sh $mp4_file
        fi
        done
    fi
}

roomFolderPath="$root_path/Videos"
while true; do
    for roomFolder in "$roomFolderPath"/*; do
        if [ -d "$roomFolder" ]; then
            check_and_process_folder "$roomFolder"
        fi
    done
    echo "$(date +"%Y-%m-%d %H:%M:%S") There is no file recorded. Check again in 120 seconds."
    sleep 120
done