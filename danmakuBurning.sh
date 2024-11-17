#!/bin/bash
# DESCRIPTION:
#   The script mainly burns danmaku into new video and delete original files, along with starting video uploading.
#
# PARAMETERS:
#   INPUT: 
#       $1 - the full path of the video
#   OUTPUT:
#       $formatVideoName - eg:/path/to/video/roomid_YYYY-MM-DD-HH.mp4

# Import the $root_path
while read key value; do
    export $key="$value"
done < ./path.txt
echo $root_path

# check if the filename is provided.
if [ $# -ne 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi

# obtain full path
full_path=$1

# delete json files
filename_without_ext="${full_path%.*}"
jsonlPath="${filename_without_ext}.jsonl"
rm $jsonlPath

# use dirname to obtain the path，along with basename to obtain filename.
path=$(dirname "$full_path")
filename=$(basename "$full_path")

# extract the extonsion name.
extension="${filename##*.}"

roomid=$(echo "$filename" | cut -d'_' -f1)
timestamp=$(echo "$filename" | cut -d'_' -f2)

# retrieve parameters. 
year=${timestamp:0:4}
month=${timestamp:4:2}
day=${timestamp:6:2}
hour=${timestamp:9:2}

# format the file name
formatVideoName="${path}/${roomid}_${year}-${month}-${day}-${hour}.${extension}"

# generate the converted subtitle file name
assPath="${path}/${roomid}_${year}-${month}-${day}-${hour}.ass"

# use DanmakuFactory to convert the xml file
xmlPath="${filename_without_ext}.xml"
if [ -f "$xmlPath" ]; then
    $root_path/DanmakuFactory -o "$assPath" -i "$xmlPath" --ignore-warnings
    rm $xmlPath
    echo “danmaku convert success!”
    export ASS_PATH="$assPath"
    python3 $root_path/removeEmojis.py > $root_path/logs/burningLog/remove-$(date +%Y%m%d%H%M%S).log 2>&1
fi

# Burn danmaku into video.
if [ -f "$assPath" ]; then
    # The only cpu version
    # ffmpeg -y -i $full_path -vf ass=$assPath -preset ultrafast $formatVideoName > $root_path/logs/burningLog/burn-$(date +%Y%m%d%H%M%S).log 2>&1
    # The Nvidia GPU accelerating version
    ffmpeg -y -hwaccel cuda -c:v h264_cuvid -i $full_path -c:v h264_nvenc -vf ass=$assPath $formatVideoName > $root_path/logs/burningLog/burn-$(date +%Y%m%d%H%M%S).log 2>&1
    rm $assPath
else
    # The only cpu version
    # ffmpeg -y -i $full_path -vf -preset ultrafast $formatVideoName > $root_path/logs/burningLog/burn-$(date +%Y%m%d%H%M%S).log 2>&1
    # The Nvidia GPU acceleting version
    ffmpeg -y -hwaccel cuda -c:v h264_cuvid -i $full_path -c:v h264_nvenc $formatVideoName > $root_path/logs/burningLog/burn-$(date +%Y%m%d%H%M%S).log 2>&1
fi

echo "ffmpeg successfully complete!"

# Delete the original video.
rm $full_path

# Upload video.
echo "$formatVideoName" >> $root_path/uploadVideoQueue.txt