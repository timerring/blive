#!/bin/bash

# 检查是否提供了文件名作为参数
if [ $# -ne 1 ]; then
    echo "Usage: $0 filename"
    exit 1
fi

# 获取完整的文件路径
full_path=$1
export full_path

filename_without_ext="${full_path%.*}"
xmlName="${filename_without_ext}.xml"

# 使用 dirname 获取路径部分，basename 获取文件名
path=$(dirname "$full_path")
filename=$(basename "$full_path")

# 提取文件扩展名
extension="${filename##*.}"

# 提取roomid和时间戳
roomid=$(echo "$filename" | cut -d'_' -f1)
timestamp=$(echo "$filename" | cut -d'_' -f2)

# 提取并格式化日期部分
year=${timestamp:0:4}
month=${timestamp:4:2}
day=${timestamp:6:2}
hour=${timestamp:9:2}

# 格式化日期和时间
formatVideoName="${path}/${roomid}_${year}-${month}-${day}-${hour}.${extension}"
export formatVideoName

formatXmlName="${path}/${roomid}_${year}-${month}-${day}-${hour}.xml"
mv $xmlName $formatXmlName

# 输出结果
#echo "$formatName"
export formatXmlName

./DanmakuProcess.sh
