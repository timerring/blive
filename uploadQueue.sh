#!/bin/bash
# DESCRIPTION:
#   Read the uploadVideoQueue and upload the first video in loop.
#
# PARAMETERS:
#   INPUT:  none.
#   OUTPUT: none.

uploadQueue="$BILIVE_PATH/uploadVideoQueue.txt"
tempQueue="$BILIVE_PATH/tempVideoQueue.txt"

processVideo() {
    local line="$1"
    $BILIVE_PATH/uploadVideo.sh "$line" > $BILIVE_PATH/logs/uploadDanmakuLog/upload-$(date +%Y%m%d%H%M%S).log 2>&1
    sleep 10
    sed -i '1d' "$uploadQueue"
}

while true; do
    uniq $uploadQueue > $tempQueue && mv $tempQueue $uploadQueue
    if [ -s "$uploadQueue" ]; then
        firstLine=$(head -n 1 "$uploadQueue")
        (processVideo "$firstLine") &
        pid=$!
        echo "uploading $firstLine，PROCESS ID $pid"
        wait $pid
        echo "$firstLine uploaded successfully!"
    else
        echo "$(date +"%Y-%m-%d %H:%M:%S")  There is no file awaiting uploading. Check again in 120 seconds."
        sleep 120
    fi
done