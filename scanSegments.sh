#!/bin/bash
# DESCRIPTION:
#   Check the recorded folders in loop, if there are the same date and hour videos, handle them in burnAndMerge script. 
#   Otherwise, handle them in danmakuBurning script.
#
# PARAMETERS:
#   INPUT:  none.
#   OUTPUT: $otherMp4File >> sameSegments.txt

CheckAndProcessFolder() {
    local folderPath=$1

    # Check if there are any flv files in the folder.
    # If exists, which means the room is under recording.
    flvFile=("$folderPath"/*.flv)
    flvName=$(basename "$flvFile")
    if [ ${#flvName} -gt 10 ]; then
        echo "Found flv files in $folderPath. Skipping."
        return
    fi

    # Check if there are video clips in the same date and hour
    for mp4File in "$folderPath"/*.mp4; do
        if [ ${#mp4File} -gt 35 ]; then
            detectName=$(basename "$mp4File")

            # This length to prevent processing the burned videos again!
            if [ ${#detectName} -gt 27 ]; then
                echo $mp4File > sameSegments.txt

                # Find the same date and hour video
                datePart=$(basename "$mp4File" | cut -d '_' -f 2| cut -d '-' -f 1)
                # If you want to merge the videos with the same date and hour, uncomment the following lines.
                # hourPart=$(basename "$mp4File" | cut -d '-' -f 2)
                for otherMp4File in "$folderPath"/*.mp4; do
                    # if [[ $(basename "$otherMp4File" | cut -d '_' -f 2| cut -d '-' -f 1) == $datePart && $(basename "$otherMp4File" | cut -d '-' -f 2) == $hourPart ]]; then
                    if [[ $(basename "$otherMp4File" | cut -d '_' -f 2| cut -d '-' -f 1) == $datePart ]]; then
                        if [[ "$mp4File" != "$otherMp4File" ]]; then
                            echo "$otherMp4File" >> sameSegments.txt
                        fi
                    fi
                done
                lineCount=$(wc -l < "sameSegments.txt")
                echo "$lineCount"
                if [ $lineCount -gt 1 ]; then
                    $BILIVE_PATH/burnAndMerge.sh sameSegments.txt
                else
                    $BILIVE_PATH/danmakuBurning.sh $mp4File
                fi
            fi
        fi
    done
}

roomFolderPath="$BILIVE_PATH/Videos"
while true; do
    for roomFolder in "$roomFolderPath"/*; do
        if [ -d "$roomFolder" ]; then
            CheckAndProcessFolder "$roomFolder"
        fi
    done
    echo "$(date +"%Y-%m-%d %H:%M:%S") There is no file recorded. Check again in 120 seconds."
    sleep 120
done