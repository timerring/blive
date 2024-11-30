#!/bin/bash
# DESCRIPTION:
#   Read the sameSegments.txt and burn them in tmp, then merge them into a integral video.
#   Eventually, add the integral video to uploadVideoQueue.
#
# PARAMETERS:
#   INPUT:  sameSegments.txt
#   OUTPUT: uploadVideoQueue.txt

firstOutputFile=""
while read -r line; do
    # Skip when read blanket line
    echo "==================== deal with $line ======================="
    if [ -z "$line" ]; then
        continue
    fi

    # Compress the danmaku into video
    outputFile=$(echo "$line" | sed 's/_\([0-9]\{4\}\)\([0-9]\{2\}\)\([0-9]\{2\}\)/_\1-\2-\3/')

    # Convert the danmaku files
    xmlPath=${line%.mp4}.xml
    assPath=${line%.mp4}.ass
    if [ -f "$xmlPath" ]; then
        python $BILIVE_PATH/src/utils/adjustPrice.py $xmlPath
        $BILIVE_PATH/src/utils/DanmakuFactory -o "$assPath" -i "$xmlPath" --msgboxfontsize 30 --msgboxsize 400x1000 --ignore-warnings
        echo "==================== generated $assPath ===================="
        export ASS_PATH="$assPath"
        python3 $BILIVE_PATH/src/utils/removeEmojis.py >> $BILIVE_PATH/logs/removeEmojis.log 2>&1
    fi
    
    # Initial some basic parameters and create tmp folder
    if [ -z "$firstOutputFile" ]; then
    dir=$(dirname $outputFile)
    firstOutputFile="${outputFile%-[0-9][0-9]-[0-9][0-9].mp4}.mp4"
    tmpDir=$dir/tmp
    mkdir -p "$tmpDir"
    echo "==================== create tmp folder $tmpDir ===================="
    fi

    # Generate the srt file via whisper model
    python $BILIVE_PATH/src/subtitle/generate.py $line > $BILIVE_PATH/logs/burningLog/whisper-$(date +%Y%m-%d-%H%M%S).log 2>&1
    srtPath="${line%.mp4}.srt"
    fileName=$(basename "$outputFile")
    newPath="$tmpDir/$fileName"
    echo "==================== burning $newPath ===================="
    echo "file '$newPath'" >> mergevideo.txt
    if [ -f "$assPath" ]; then
        # The only cpu version.
        # ffmpeg -i "$line" -vf "ass=$assPath" -preset ultrafast "$newPath" -y -nostdin  > $BILIVE_PATH/logs/burningLog/burn-$(date +%Y%m-%d-%H%M%S).log 2>&1
        # The Nvidia GPU accelerating version.
        # ffmpeg -hwaccel cuda -c:v h264_cuvid -i "$line" -c:v h264_nvenc -vf "ass=$assPath" "$newPath" -y -nostdin > $BILIVE_PATH/logs/burningLog/burn-$(date +%Y%m-%d-%H%M%S).log 2>&1
        # The Nvidia GPU accelerate subtitles and danmaku burning
        # If you don't have NVIDIA GPU, please comment the command below directly, and use the cpu version above.
        ffmpeg -hwaccel cuda -c:v h264_cuvid -i "$line" -c:v h264_nvenc -vf "subtitles=$srtPath,subtitles=$assPath" "$newPath" -y -nostdin > $BILIVE_PATH/logs/burningLog/burn-$(date +%Y%m-%d-%H%M%S).log 2>&1
    else
        # The only cpu version.
        # ffmpeg -i "$line" -vf -preset ultrafast "$newPath" -y -nostdin  > $BILIVE_PATH/logs/burningLog/burn-$(date +%Y%m-%d-%H%M%S).log 2>&1
        # The Nvidia GPU accelerating version.
        ffmpeg -hwaccel cuda -c:v h264_cuvid -i "$line" -c:v h264_nvenc -vf "subtitles=$srtPath" "$newPath" -y -nostdin > $BILIVE_PATH/logs/burningLog/burn-$(date +%Y%m-%d-%H%M%S).log 2>&1
    fi
    
    # Delete the related items of videos
    # rm ${line%.mp4}.*
    mv $line ${line%.mp4}
done < ./src/sameSegments.txt

rm $BILIVE_PATH/src/sameSegments.txt
# merge the videos
echo "==================== merge starts ===================="
# echo "ffmpeg -f concat -i mergevideo.txt -c copy $firstOutputFile"
ffmpeg -f concat -safe 0 -i mergevideo.txt -use_wallclock_as_timestamps 1 -c copy $firstOutputFile > $BILIVE_PATH/logs/mergeLog/merge-$(date +%Y%m-%d-%H%M%S).log 2>&1

# delete useless videos and lists
# rm -r $tmpDir
rm mergevideo.txt
# rm $firstOutputFile

echo "==================== add $videoUploadPath to upload queue ===================="
echo "$firstOutputFile" >> $BILIVE_PATH/src/uploadProcess/uploadVideoQueue.txt
