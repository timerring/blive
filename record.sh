# pause#!/bin/bash
export config=./settings.toml

# Do not use proxy
export no_proxy=*

# bind host and port (can edit)
host=0.0.0.0
port=2233

kill -9 $(pgrep -f blrec)
nohup blrec -c $config --open --host $host --port $port > $BILIVE_PATH/logs/blrec.log 2>&1 &
echo "blrec run success!"