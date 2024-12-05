# kill the previous scanSegments process
kill -9 $(pgrep -f scanSegments)
# start the scanSegments process
nohup $BILIVE_PATH/src/scanSegments.sh > $BILIVE_PATH/logs/scanSegments.log 2>&1 &
# Check if the last command was successful
if [ $? -eq 0 ]; then
    echo "success"
else
    echo "An error occurred while starting scanSegments. Check the logs for details."
fi