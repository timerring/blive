#!/bin/bash

# 构建新的文件名
assName=${formatXmlName%.xml}.ass

export assName

#use DanmakuFactory to convert the xml File
/root/blive/DanmakuFactory/DanmakuFactory -o "$assName" -i "$formatXmlName"

#echo "$full_path"
#echo "$assName"
#echo "$formatVideoName"

ffmpeg -i $full_path -vf ass=$assName $formatVideoName

echo "ffmpeg Successfully complete!"

./upload.sh $formatVideoName
