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
$root_path/DanmakuFactory -o "$assPath" -i "$xmlPath"
rm $xmlPath
echo “danmaku convert success!”


# Burn danmaku into video.
ffmpeg -y -i $full_path -vf ass=$assPath -preset ultrafast $formatVideoName > $root_path/logs/burningLog/burn-$(date +%Y%m%d%H%M%S).log 2>&1
echo "ffmpeg successfully complete!"

# Delete the original video.
rm $full_path
rm $assPath

# Upload video.
echo "$formatVideoName" >> $root_path/uploadVideoQueue.txt