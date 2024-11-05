#!/bin/bash
# DESCRIPTION:
#   Check the recorded folders in loop, if there are the same date and hour videos, handle them in burnAndMerge script. 
#   Otherwise, handle them in danmakuBurning script.
#
# PARAMETERS:
#   INPUT:  none.
#   OUTPUT: $same_date_videos > sameSegments.txt

while read key value; do
    export $key="$value"
done < ./path.txt
echo $root_path

check_and_process_folder() {
    local folder_path=$1
    local count=0
    local same_date_hour_videos=""
    local formatted_date_hour=""

    # Check if there are any flv files in the folder.
    # If exists, which means the room is under recording.
    flv_file=("$folder_path"/*.flv)
    flv_name=$(basename "$flv_file")
    if [ ${#flv_name} -gt 10 ]; then
        echo "Found flv files in $folder_path. Skipping."
        return
    fi

    # Check if there are video clips in the same date and hour
    for mp4_file in "$folder_path"/*.mp4; do
        if [ ${#mp4_file} -gt 35 ]; then
            detect_name=$(basename "$mp4_file")

            # This length to prevent processing the burned videos again!
            if [ ${#detect_name} -gt 27 ]; then
                echo $mp4_file > sameSegments.txt

                # Find the same date and hour video
                date_part=$(basename "$mp4_file" | cut -d '_' -f 2| cut -d '-' -f 1)
                hour_part=$(basename "$mp4_file" | cut -d '-' -f 2)
                for other_mp4_file in "$folder_path"/*.mp4; do
                    if [[ $(basename "$other_mp4_file" | cut -d '_' -f 2| cut -d '-' -f 1) == $date_part && $(basename "$other_mp4_file" | cut -d '-' -f 2) == $hour_part ]]; then
                        if [[ "$mp4_file" != "$other_mp4_file" ]]; then
                            echo "$other_mp4_file" >> sameSegments.txt
                        fi
                    fi
                done
                line_count=$(wc -l < "sameSegments.txt")
                echo "$line_count"
                if [ $line_count -gt 1 ]; then
                    $root_path/burningAndMerge.sh sameSegments.txt
                else
                    $root_path/danmakuBurning.sh $mp4_file
                fi
            fi
        fi
    done
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