#!/bin/bash

# check whether the filename is provided.
if [ $# -ne 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi

# require the full path
full_path=$1
export full_path

filename_without_ext="${full_path%.*}"
xmlName="${filename_without_ext}.xml"

# Use dirname to get the path part，and use basename to get the filename
path=$(dirname "$full_path")
filename=$(basename "$full_path")

# get the extended name
extension="${filename##*.}"

# extract roomid and timestamp
roomid=$(echo "$filename" | cut -d'_' -f1)
timestamp=$(echo "$filename" | cut -d'_' -f2)

# extract and format the time variables.
year=${timestamp:0:4}
month=${timestamp:4:2}
day=${timestamp:6:2}
hour=${timestamp:9:2}

# rebuilt the name of video
formatVideoName="${path}/${roomid}_${year}-${month}-${day}-${hour}.${extension}"
export formatVideoName

formatXmlName="${path}/${roomid}_${year}-${month}-${day}-${hour}.xml"
mv $xmlName $formatXmlName

#echo "$formatName"
export formatXmlName

./DanmakuProcess.sh
