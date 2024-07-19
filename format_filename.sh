#!/bin/bash

# 检查是否提供了参数
if [ $# -ne 1 ]; then
    echo "使用方法：$0 <文件名>"
    echo "文件名格式：原始ID_YYYYMMDD-HH-MI-SS.mp4"
    exit 1
fi

# 文件名
file_name=$1

# 提取原始ID、年份、月份、日期和小时
if [[ $file_name =~ ^([0-9]+)_([0-9]{4})([0-9]{2})([0-9]{2})-([0-9]{2}) ]]; then
    original_id="${BASH_REMATCH[1]}"
    year="${BASH_REMATCH[2]}"
    month="${BASH_REMATCH[3]}"
    day="${BASH_REMATCH[4]}"
    hour="${BASH_REMATCH[5]}"
else
    echo "文件名格式不正确，应为 原始ID_YYYYMMDD-HH-MI-SS.mp4"
    exit 1
fi

# 重新组合文件名
new_file_name="${original_id}_${year}-${month}-${day}-${hour}.mp4"

# 输出新的文件名
echo "新的文件名：$new_file_name"

# 可选：重命名文件
# mv "$file_name" "$new_file_name"
