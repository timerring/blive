#!/bin/bash

# check the parameter
if [ $# -ne 1 ]; then
    echo "使用方法：$0 <文件的完整路径>"
    echo "文件路径格式：/root/blive/Videos/<roomid>/<roomid>_YYYY-MM-DD-HH.mp4"
    exit 1
fi

# get full path
full_path=$1

# remove .mp4 extended name，only reserve the path and filename（without extended name）
trimmed_path="${full_path%.mp4}"


# extract the roomid and time
# Use parameter expansion to remove the path, leaving only the file name
file_name=$(basename "$full_path")
# Regular expression to match room number and date time
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

# print the roomid and time variables.
echo "房间号: $room_id"
echo "年份: $year"
echo "月份: $month"
echo "日期: $day"
echo "小时: $hour"

# define the path of yaml file
yaml_file="./config/$room_id.yaml"

# check if the file is exist.
if [ ! -f "$yaml_file" ]; then
    echo "文件 $yaml_file 不存在。"
    exit 1
fi

# define the path of copy of the yaml
copy_yaml_file="./config/copy_$room_id.yaml"

# make a copy of yaml
cp "$yaml_file" "$copy_yaml_file"

# Replace date and time parameters in a copy file with the sed command
sed -i "s/%Y/$year/g" "$copy_yaml_file"
sed -i "s/%M/$month/g" "$copy_yaml_file"
sed -i "s/%d/$day/g" "$copy_yaml_file"
sed -i "s/%H/$hour/g" "$copy_yaml_file"
sed -i "s/%P/${file_name}/g" "$copy_yaml_file"

echo "文件名 $file_name 中日期和时间参数已更新到 $copy_yaml_file"

# Execute the upload command and check if it was successful
if ./biliup upload "$full_path" --config "$copy_yaml_file"; then
    echo "上传成功，正在删除相关文件 $trimmed_path*"
    rm $trimmed_path*
    echo "相关文件已被删除。"
else
    echo "上传失败，相关文件保留。"
    exit 1
fi
