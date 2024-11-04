#!/bin/bash
# DESCRIPTION:
#   Read the sameVideos.txt and burn them in tmp, then merge them into a integral video.
#   Eventually, add the integral video to uploadVideoQueue.
#
# PARAMETERS:
#   INPUT:  sameVideos.txt
#   OUTPUT: uploadVideoQueue.txt

# Import the $root_path
while read key value; do
    export $key="$value"
done < ./path.txt
echo $root_path

first_output_file=""
while read -r line; do
    # Skip when read blanket line
    echo "==================== deal with $line ======================="
    if [ -z "$line" ]; then
        continue
    fi

    # Convert the danmaku files
    xml_file=${line%.mp4}.xml
    ass_file=${line%.mp4}.ass
    $root_path/DanmakuFactory -o "$ass_file" -i "$xml_file" --ignore-warnings
    echo "==================== generated $ass_file ===================="

    # Compress the danmaku into video
    output_file=$(echo "$line" | sed 's/_\([0-9]\{4\}\)\([0-9]\{2\}\)\([0-9]\{2\}\)/_\1-\2-\3/')
    
    # Initial some basic parameters
    if [ -z "$first_output_file" ]; then
    dir=$(dirname $output_file)
    first_output_file="${output_file%-[0-9][0-9]-[0-9][0-9].mp4}.mp4"
    tmp_dir=$dir/tmp
    mkdir -p "$tmp_dir"
    echo "==================== create tmp folder $tmp_dir ===================="
    fi

    file_name=$(basename "$output_file")
    new_path="$tmp_dir/$file_name"
    echo "==================== compress $new_path ===================="
    echo "file '$new_path'" >> mergevideo.txt
    # echo "ffmpeg -i $line -vf ass=$ass_file $output_file"
    ffmpeg -i "$line" -vf "ass=$ass_file" -preset ultrafast "$new_path" -y -nostdin  > $root_path/logs/burningLog/burn-$(date +%Y%m%d%H%M%S).log 2>&1
    # Delete the related items of videos
    rm ${line%.mp4}.*
done < sameVideos.txt

# merge the videos
echo "==================== merge starts ===================="
# echo "ffmpeg -f concat -i mergevideo.txt -c copy $first_output_file"
ffmpeg -f concat -safe 0 -i mergevideo.txt -use_wallclock_as_timestamps 1 -c copy $first_output_file > $root_path/logs/mergeLog/merge-$(date +%Y%m%d%H%M%S).log 2>&1

# delete useless videos and lists
rm -r $tmp_dir
rm mergevideo.txt

echo "==================== start upload $first_output_file ===================="
# echo "nohup /root/blive/uploadVideo.sh $first_output_file > /root/blive/logs/uploadDanmakuLog/$(date +%Y%m%d%H%M%S).log 2>&1 &"
echo "$first_output_file" >> $root_path/uploadVideoQueue.txt
echo "==================== OVER ===================="