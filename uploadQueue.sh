#!/bin/bash
# DESCRIPTION:
#   Read the uploadVideoQueue and upload the first video in loop.
#
# PARAMETERS:
#   INPUT:  none.
#   OUTPUT: none.

while read key value; do
    export $key="$value"
done < ./path.txt
echo $root_path

uploadQueue="$root_path/uploadVideoQueue.txt"

processVideo() {
    local line="$1"
    $root_path/uploadVideo.sh "$line" > $root_path/logs/uploadDanmakuLog/upload-$(date +%Y%m%d%H%M%S).log 2>&1
    sleep 10
    sed -i '1d' "$uploadQueue"
}

while true; do
    if [ -s "$uploadQueue" ]; then
        firstLine=$(head -n 1 "$uploadQueue")
        (processVideo "$firstLine") &
        pid=$!
        echo "uploading $firstLine，PROCESS ID $pid"
        wait $pid
        echo "$firstLine uploaded successfully!"
    else
        echo "There is no file awaiting uploading. Check again in 120 seconds."
        sleep 120
    fi
done