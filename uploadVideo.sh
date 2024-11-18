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

# check if the provided parameters
# specific for manually upload single video
if [ $# -ne 1 ]; then
    echo "Please provide the full path of mp4"
    echo "for example：$rootPath/Videos/<roomid>/<roomid>_YYYY-MM-DD-HH.mp4"
    exit 1
fi

uploadPath=$1

# extract the roomid and date information
fileName=$(basename "$uploadPath")
# use regular expression to match the parameter
if [[ $fileName =~ ([0-9]+)_([0-9]{4})-([0-9]{2})-([0-9]{2})-([0-9]{2})\.mp4 ]]; then
    roomID="${BASH_REMATCH[1]}"
    year="${BASH_REMATCH[2]}"
    month="${BASH_REMATCH[3]}"
    day="${BASH_REMATCH[4]}"
    hour="${BASH_REMATCH[5]}"
else
    echo "Please check if the file name conforms <roomid>_YYYY-MM-DD-HH.mp4"
    exit 1
fi

# print the extracted info
echo "roomid: $roomID"
echo "year: $year"
echo "month: $month"
echo "day: $day"
echo "hour: $hour"

# In this part, I make common templates of yaml as original, and try to substitue the parameters in 
# original to create the copy. Then upload videos via the copy.

# define the path of yaml.
yamlFile="$rootPath/biliup/config/$roomID.yaml"

# check if the yaml is exist.
if [ ! -f "$yamlFile" ]; then
    echo "$yamlFile is not exist."
    exit 1
fi

# define the path of copy.
copy_yamlFile="$rootPath/biliup/config/copy_$roomID.yaml"

# make the copy.
cp "$yamlFile" "$copy_yamlFile"

# substitue the parameters in the copy file.
sed -i "s/%Y/$year/g" "$copy_yamlFile"
sed -i "s/%M/$month/g" "$copy_yamlFile"
sed -i "s/%d/$day/g" "$copy_yamlFile"
sed -i "s/%H/$hour/g" "$copy_yamlFile"
sed -i "s/%P/${fileName}/g" "$copy_yamlFile"

echo "The parameters have been updated in the $copy_yamlFile"

# Use biliup tool to upload video, and then delete the subtitle ass file and video file.
if $rootPath/biliup/biliup upload "$uploadPath" --config "$copy_yamlFile"; then
    echo "Upload successfully，then delete the video"
    rm $uploadPath
else
    echo "Fail to upload, the files will be reserve."
    exit 1
fi