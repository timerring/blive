#!/bin/bash

# generate the converted subtitle file name
assName=${formatXmlName%.xml}.ass

export assName

# use DanmakuFactory to convert the xml file
/root/blive/DanmakuFactory/DanmakuFactory -o "$assName" -i "$formatXmlName"

#echo "$full_path"
#echo "$assName"
#echo "$formatVideoName"

ffmpeg -i $full_path -vf ass=$assName $formatVideoName

echo "ffmpeg successfully complete!"

./uploadVideo.sh $formatVideoName
