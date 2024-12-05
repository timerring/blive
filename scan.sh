# kill the previous scanSegments process
kill -9 $(pgrep -f src.burn.scan)
# start the scanSegments process
nohup python -m src.burn.scan > $BILIVE_PATH/logs/scanLog/scan-$(date +%Y%m%d-%H%M%S).log 2>&1 &
# Check if the last command was successful
if [ $? -eq 0 ]; then
    echo "success"
else
    echo "An error occurred while starting scanSegments. Check the logs for details."
fi