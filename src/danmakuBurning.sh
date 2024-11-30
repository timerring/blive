#!/bin/bash
# DESCRIPTION:
#   The script mainly burns danmaku into new video and delete original files, along with starting video uploading.
#
# PARAMETERS:
#   INPUT: 
#       $1 - the full path of the video
#   OUTPUT:
#       $formatVideoName - eg:/path/to/video/roomid_YYYY-MM-DD-HH.mp4

# check if the filename is provided.
if [ $# -ne 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi

# obtain full path
fullPath=$1

# use dirname to obtain the path，along with basename to obtain filename.
path=$(dirname "$fullPath")
filename=$(basename "$fullPath")

roomid=$(echo "$filename" | cut -d'_' -f1)
timestamp=$(echo "$filename" | cut -d'_' -f2)

# retrieve parameters. 
year=${timestamp:0:4}
month=${timestamp:4:2}
day=${timestamp:6:2}
hour=${timestamp:9:2}

# format the file name
formatVideoName="${path}/${roomid}_${year}-${month}-${day}-${hour}.mp4"

# Process the xml file as ass file
assPath="${fullPath%.*}.ass"
xmlPath="${fullPath%.*}.xml"
if [ -f "$xmlPath" ]; then
    python $BILIVE_PATH/src/utils/adjustPrice.py $xmlPath
    $BILIVE_PATH/src/utils/DanmakuFactory -o "$assPath" -i "$xmlPath" --msgboxfontsize 30 --ignore-warnings
    echo “danmaku convert success!”
    export ASS_PATH="$assPath"
    python3 $BILIVE_PATH/src/utils/removeEmojis.py >> $BILIVE_PATH/logs/removeEmojis.log 2>&1
fi

# Generate the srt file via whisper model
# If you don't have NVIDIA GPU, please comment the command below directly.
python $BILIVE_PATH/src/subtitle/generate.py $fullPath > $BILIVE_PATH/logs/burningLog/whisper-$(date +%Y%m-%d-%H%M%S).log 2>&1
srtPath="${fullPath%.*}.srt"

# Burn danmaku into video.
if [ -f "$assPath" ]; then
    # The only cpu version
    # ffmpeg -y -i $fullPath -vf ass=$assPath -preset ultrafast $formatVideoName > $BILIVE_PATH/logs/burningLog/burn-$(date +%Y%m-%d-%H%M%S).log 2>&1
    # The Nvidia GPU accelerating version
    # ffmpeg -y -hwaccel cuda -c:v h264_cuvid -i $fullPath -c:v h264_nvenc -vf ass=$assPath $formatVideoName > $BILIVE_PATH/logs/burningLog/burn-$(date +%Y%m-%d-%H%M%S).log 2>&1
    # The Nvidia GPU accelerate subtitles and danmaku burning
    # If you don't have NVIDIA GPU, please comment the command below directly, and use the cpu version above.
    ffmpeg -y -hwaccel cuda -c:v h264_cuvid -i $fullPath -c:v h264_nvenc -vf "subtitles=$srtPath,subtitles=$assPath" $formatVideoName > $BILIVE_PATH/logs/burningLog/burn-$(date +%Y%m-%d-%H%M%S).log 2>&1
else
    # If the ass file is not found, only burn the subtitles into video.
    # Nonetheless, If you don't have Nvidia GPU to burn the subtitles, please comment the command below directly.
    # The Nvidia GPU accelerate subtitles burning
    ffmpeg -y -hwaccel cuda -c:v h264_cuvid -i $fullPath -c:v h264_nvenc -vf "subtitles=$srtPath" $formatVideoName > $BILIVE_PATH/logs/burningLog/burn-$(date +%Y%m-%d-%H%M%S).log 2>&1
fi

echo "ffmpeg successfully complete!"

# Delete the original video.
rm $fullPath
rm $xmlPath
rm $assPath
rm $srtPath
# mv $fullPath ${fullPath%.*}

echo "==================== add $formatVideoName to upload queue ===================="
echo "$formatVideoName" >> $BILIVE_PATH/src/uploadProcess/uploadVideoQueue.txt
