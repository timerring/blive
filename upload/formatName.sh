#!/bin/bash

# check if the filename is provided.
if [ $# -ne 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi

# obtain full path
full_path=$1
export full_path

filename_without_ext="${full_path%.*}"
xmlName="${filename_without_ext}.xml"

# use dirname to obtain the path，along with basename to obtain filename.
path=$(dirname "$full_path")
filename=$(basename "$full_path")

# extract the extonsion name.
extension="${filename##*.}"

roomid=$(echo "$filename" | cut -d'_' -f1)
timestamp=$(echo "$filename" | cut -d'_' -f2)

# retrieve parameters. 
year=${timestamp:0:4}
month=${timestamp:4:2}
day=${timestamp:6:2}
hour=${timestamp:9:2}

# format the file name
formatVideoName="${path}/${roomid}_${year}-${month}-${day}-${hour}.${extension}"
export formatVideoName

formatXmlName="${path}/${roomid}_${year}-${month}-${day}-${hour}.xml"
mv $xmlName $formatXmlName

#echo "$formatName"
export formatXmlName

./danmakuProcess.sh
