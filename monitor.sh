#!/bin/bash
# DESCRIPTION:
#   The script mainly monitor the video folders and execute the recorded videos.
#
# PARAMETERS:
#   INPUT: 
#       $1 - the video folder (default: $rootPath/Videos)
#   OUTPUT: none.

while read key value; do
    export $key="$value"
done < ./path.txt

if [ $# -gt 0 ]; then
    targetFolder="$1"
else
    targetFolder="$rootPath/Videos"
fi


function MonitorDirectory() {
    for file in "$1"/*; do
        if [ -d "$file" ]; then
            MonitorDirectory "$file"
        elif [[ "$file" == *.mp4 ]]; then
            $rootPath/danmakuBurning.sh $file
        fi
    done
}

while true; do
    MonitorDirectory "$targetFolder"
    sleep 180
done