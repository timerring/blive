#!/bin/bash

# generate the converted subtitle file name
assName=${formatXmlName%.xml}.ass

export assName

# use DanmakuFactory to convert the xml file
/root/blive/DanmakuFactory/DanmakuFactory -o "$assName" -i "$formatXmlName"

echo “danmaku convert success! And will delete xml files...”

rm $formatXmlName
# echo "$full_path"
# echo "$assName"
# echo "$formatVideoName"

ffmpeg -i $full_path -vf ass=$assName $formatVideoName

echo "ffmpeg successfully complete! And will delete danmaku files..."

rm $assName

./uploadVideo.sh $formatVideoName