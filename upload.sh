# kill the previous scanSegments process
kill -9 $(pgrep -f upload)
kill -9 $(pgrep -f biliup)
# start the scanSegments process
# nohup $BILIVE_PATH/src/upload/uploadQueue.sh > $BILIVE_PATH/logs/uploadQueue.log 2>&1 &
nohup python -m src.upload.upload > $BILIVE_PATH/logs/uploadLog/upload-$(date +%Y%m%d-%H%M%S).log 2>&1 &
# Check if the last command was successful
if [ $? -eq 0 ]; then
    echo "success"
else
    echo "An error occurred while starting upload. Check the logs for details."
fi