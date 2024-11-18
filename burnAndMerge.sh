#!/bin/bash
# DESCRIPTION:
#   Read the sameSegments.txt and burn them in tmp, then merge them into a integral video.
#   Eventually, add the integral video to uploadVideoQueue.
#
# PARAMETERS:
#   INPUT:  sameSegments.txt
#   OUTPUT: uploadVideoQueue.txt

# Import the $rootPath
while read key value; do
    export $key="$value"
done < ./path.txt

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
    xmlFile=${line%.mp4}.xml
    echo "Read the $xmlFile"
    assFile=${line%.mp4}.ass
    echo "Read the $assFile"
    if [ -f "$xmlFile" ]; then
        $rootPath/DanmakuFactory -o "$assFile" -i "$xmlFile" --ignore-warnings
        echo "==================== generated $assFile ===================="
        export ASS_PATH="$assFile"
        python3 $rootPath/removeEmojis.py > $rootPath/logs/burningLog/remove-$(date +%Y%m%d%H%M%S).log 2>&1
    fi
    
    # Initial some basic parameters and create tmp folder
    if [ -z "$firstOutputFile" ]; then
    dir=$(dirname $outputFile)
    firstOutputFile="${outputFile%-[0-9][0-9]-[0-9][0-9].mp4}.mp4"
    tmpDir=$dir/tmp
    mkdir -p "$tmpDir"
    echo "==================== create tmp folder $tmpDir ===================="
    fi

    fileName=$(basename "$outputFile")
    newPath="$tmpDir/$fileName"
    echo "==================== burning $newPath ===================="
    echo "file '$newPath'" >> mergevideo.txt
    if [ -f "$assFile" ]; then
        # echo "ffmpeg -i $line -vf ass=$assFile $outputFile"
        ffmpeg -hwaccel cuda -c:v h264_cuvid -i "$line" -c:v h264_nvenc -vf "ass=$assFile" "$newPath" -y -nostdin > $rootPath/logs/burningLog/burn-$(date +%Y%m%d%H%M%S).log 2>&1
        # ffmpeg -i "$line" -vf "ass=$assFile" -preset ultrafast "$newPath" -y -nostdin  > $rootPath/logs/burningLog/burn-$(date +%Y%m%d%H%M%S).log 2>&1
    else
        ffmpeg -hwaccel cuda -c:v h264_cuvid -i "$line" -c:v h264_nvenc "$newPath" -y -nostdin > $rootPath/logs/burningLog/burn-$(date +%Y%m%d%H%M%S).log 2>&1
        # ffmpeg -i "$line" -vf -preset ultrafast "$newPath" -y -nostdin  > $rootPath/logs/burningLog/burn-$(date +%Y%m%d%H%M%S).log 2>&1
    fi
    
    # Delete the related items of videos
    rm ${line%.mp4}.*
done < sameSegments.txt

# merge the videos
echo "==================== merge starts ===================="
# echo "ffmpeg -f concat -i mergevideo.txt -c copy $firstOutputFile"
ffmpeg -f concat -safe 0 -i mergevideo.txt -use_wallclock_as_timestamps 1 -c copy $firstOutputFile > $rootPath/logs/mergeLog/merge-$(date +%Y%m%d%H%M%S).log 2>&1

# delete useless videos and lists
rm -r $tmpDir
rm mergevideo.txt

echo "==================== start upload $firstOutputFile ===================="
# echo "nohup /root/blive/uploadVideo.sh $firstOutputFile > /root/blive/logs/uploadDanmakuLog/$(date +%Y%m%d%H%M%S).log 2>&1 &"
echo "$firstOutputFile" >> $rootPath/uploadVideoQueue.txt
echo "==================== OVER ===================="