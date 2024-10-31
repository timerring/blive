#!/bin/bash

root_path=$(pwd)

rm ./path.txt
echo "root_path $root_path" >> ./path.txt

# hh=$1
# mm=$2

# # define the commmand
# task_command="$mm $hh * * * cd $root_path;./readLocalPath.sh"

# # delete the former task
# crontab -r
# (crontab -l ; echo "$task_command") | crontab -

# echo "Add task successfully."