# pause#!/bin/bash

# 配置文件
export config=./settings.toml

# 不使用代理
export no_proxy=*

# 主机和端口绑定，可以按需修改。
host=0.0.0.0
port=2233

nohup blrec -c $config --open --host $host --port $port > blrec.log 2>&1 &
echo "blrec run success!"

nohup python3 main.py > auto_upload.log 2>&1 &
echo "auto_upload run success!"
