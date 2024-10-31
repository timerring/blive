# pause#!/bin/bash
nohup python3 main.py > auto_upload.log 2>&1 &
echo "auto_upload run success!"