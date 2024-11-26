# pause#!/bin/bash
nohup python3 $BILIVE_PATH/src/main.py > $BILIVE_PATH/logs/uploadNoDanmaku.log 2>&1 &
echo "auto upload without danmaku run success!"