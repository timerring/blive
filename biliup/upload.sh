#!/bin/bash

# 检查是否提供了参数
if [ $# -ne 1 ]; then
    echo "使用方法：$0 <文件的完整路径>"
    echo "文件路径格式：/root/blive/Videos/<roomid>/<roomid>_YYYY-MM-DD-HH.mp4"
    exit 1
fi

# 文件的完整路径
full_path=$1

# 去除.mp4扩展名，只保留路径和文件名（不包括扩展名）
trimmed_path="${full_path%.mp4}"


# 提取房间号和日期时间
# 使用参数扩展来去除路径，只留下文件名
file_name=$(basename "$full_path")

# 正则表达式匹配房间号和日期时间
if [[ $file_name =~ ([0-9]+)_([0-9]{4})-([0-9]{2})-([0-9]{2})-([0-9]{2})\.mp4 ]]; then
    room_id="${BASH_REMATCH[1]}"
    year="${BASH_REMATCH[2]}"
    month="${BASH_REMATCH[3]}"
    day="${BASH_REMATCH[4]}"
    hour="${BASH_REMATCH[5]}"
else
    echo "文件名格式不正确，应为 <roomid>_YYYY-MM-DD-HH.mp4"
    exit 1
fi

# 显示提取的房间号和日期时间
echo "房间号: $room_id"
echo "年份: $year"
echo "月份: $month"
echo "日期: $day"
echo "小时: $hour"

# 定义原始YAML文件路径
yaml_file="./config/$room_id.yaml"

# 检查文件是否存在
if [ ! -f "$yaml_file" ]; then
    echo "文件 $yaml_file 不存在。"
    exit 1
fi

# 定义副本文件路径
copy_yaml_file="./config/copy_$room_id.yaml"

# 复制原始YAML文件到副本文件
cp "$yaml_file" "$copy_yaml_file"

# 使用sed命令在副本文件中替换日期和时间参数
sed -i "s/%Y/$year/g" "$copy_yaml_file"
sed -i "s/%M/$month/g" "$copy_yaml_file"
sed -i "s/%d/$day/g" "$copy_yaml_file"
sed -i "s/%H/$hour/g" "$copy_yaml_file"

echo "文件名 $file_name 中日期和时间参数已更新到 $copy_yaml_file"

# 执行上传命令，并检查是否成功
if ./biliup upload "$full_path" --config "$copy_yaml_file"; then
    echo "上传成功，正在删除相关文件 $trimmed_path*"
    rm $trimmed_path*
    echo "相关文件已被删除。"
else
    echo "上传失败，相关文件保留。"
    exit 1
fi
