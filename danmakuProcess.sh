#!/bin/bash
# import the $root_path
while read key value; do
    export $key="$value"
done < ./path.txt

echo $root_path

# generate the converted subtitle file name
assName=${formatXmlName%.xml}.ass

export assName

# use DanmakuFactory to convert the xml file
$root_path/DanmakuFactory/DanmakuFactory -o "$assName" -i "$formatXmlName"

echo “danmaku convert success! And will delete xml files...”

rm $formatXmlName

# echo "$full_path"
# echo "$assName"
# echo "$formatVideoName"

ffmpeg -y -i $full_path -vf ass=$assName $formatVideoName 

echo "ffmpeg successfully complete! And will delete danmaku files..."

# delete the original video and original
rm $full_path

# delete the original jsonl danmaku files
filename_without_ext="${full_path%.*}"
jsonlPath="${filename_without_ext}.jsonl"
rm $jsonlPath

rm $assName

$root_path/uploadVideo.sh $formatVideoName
