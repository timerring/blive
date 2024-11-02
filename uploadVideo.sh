#!/bin/bash

# DESCRIPTION:
#   The script mainly centers on the video uploading process along with deleting video if goes well.
#
# PARAMETERS:
#   INPUT: 
#       $uploadPath - eg:/path/to/video/roomid_YYYY-MM-DD-HH.mp4
#   OUTPUT: none

while read key value; do
    export $key="$value"
done < ./path.txt

echo $root_path

# check if the provided parameters
# specific for manually upload single video
if [ $# -ne 1 ]; then
    echo "Please provide the full path of mp4"
    echo "for example：$root_path/Videos/<roomid>/<roomid>_YYYY-MM-DD-HH.mp4"
    exit 1
fi

uploadPath=$1

# extract the roomid and date information
file_name=$(basename "$uploadPath")
# use regular expression to match the parameter
if [[ $file_name =~ ([0-9]+)_([0-9]{4})-([0-9]{2})-([0-9]{2})-([0-9]{2})\.mp4 ]]; then
    room_id="${BASH_REMATCH[1]}"
    year="${BASH_REMATCH[2]}"
    month="${BASH_REMATCH[3]}"
    day="${BASH_REMATCH[4]}"
    hour="${BASH_REMATCH[5]}"
else
    echo "Please check if the file name conforms <roomid>_YYYY-MM-DD-HH.mp4"
    exit 1
fi

# print the extracted info
echo "roomid: $room_id"
echo "year: $year"
echo "month: $month"
echo "day: $day"
echo "hour: $hour"

# In this part, I make common templates of yaml as original, and try to substitue the parameters in 
# original to create the copy. Then upload videos via the copy.

# define the path of yaml.
yaml_file="$root_path/biliup/config/$room_id.yaml"

# check if the yaml is exist.
if [ ! -f "$yaml_file" ]; then
    echo "$yaml_file is not exist."
    exit 1
fi

# define the path of copy.
copy_yaml_file="$root_path/biliup/config/copy_$room_id.yaml"

# make the copy.
cp "$yaml_file" "$copy_yaml_file"

# substitue the parameters in the copy file.
sed -i "s/%Y/$year/g" "$copy_yaml_file"
sed -i "s/%M/$month/g" "$copy_yaml_file"
sed -i "s/%d/$day/g" "$copy_yaml_file"
sed -i "s/%H/$hour/g" "$copy_yaml_file"
sed -i "s/%P/${file_name}/g" "$copy_yaml_file"

echo "The parameters have been updated in the $copy_yaml_file"

# Use biliup tool to upload video, and then delete the subtitle ass file and video file.
if $root_path/biliup/biliup upload "$uploadPath" --config "$copy_yaml_file"; then
    echo "Upload successfully，then delete the video"
    rm $uploadPath
else
    echo "Fail to upload, the files will be reserve."
    exit 1
fi