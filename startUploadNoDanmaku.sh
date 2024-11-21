# pause#!/bin/bash
nohup python3 $rootPath/main.py > $rootPath/logs/uploadNoDanmaku.log 2>&1 &
echo "auto upload without danmaku run success!"