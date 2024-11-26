# kill the previous scanSegments process
kill -9 $(pgrep -f uploadQueue)
# start the scanSegments process
nohup $BILIVE_PATH/uploadQueue.sh > $BILIVE_PATH/logs/uploadQueue.log 2>&1 &
# Check if the last command was successful
if [ $? -eq 0 ]; then
    echo "success"
else
    echo "An error occurred while starting uploadQueue. Check the logs for details."
fi