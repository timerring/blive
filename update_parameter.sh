#!/bin/bash

# 检查是否提供了参数
if [ $# -ne 1 ]; then
    echo "使用方法：$0 <文件名>"
    echo "文件名格式：roomid_YYYY-MM-DD-HH.mp4"
    exit 1
fi

# 文件名
file_name=$1

# 提取日期和时间
if [[ $file_name =~ ^[^_]+_([0-9]{4})-([0-9]{2})-([0-9]{2})-([0-9]{2})\.mp4$ ]]; then
    year="${BASH_REMATCH[1]}"
    month="${BASH_REMATCH[2]}"
    day="${BASH_REMATCH[3]}"
    hour="${BASH_REMATCH[4]}"
else
    echo "文件名格式不正确，应为 roomid_YYYY-MM-DD-HH.mp4"
    exit 1
fi

# 定义原始YAML文件路径
yaml_file="streamers.yaml"

# 检查文件是否存在
if [ ! -f "$yaml_file" ]; then
    echo "文件 $yaml_file 不存在。"
    exit 1
fi

# 定义副本文件路径
copy_yaml_file="streamers_copy.yaml"

# 复制原始YAML文件到副本文件
cp "$yaml_file" "$copy_yaml_file"

# 使用sed命令在副本文件中替换日期和时间参数
sed -i "s/%Y/$year/g" "$copy_yaml_file"
sed -i "s/%M/$month/g" "$copy_yaml_file"
sed -i "s/%d/$day/g" "$copy_yaml_file"
sed -i "s/%H/$hour/g" "$copy_yaml_file"

echo "文件名 $file_name 中的日期和时间参数已更新到 $copy_yaml_file"
