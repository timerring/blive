while read key value; do
    export $key="$value"
done < ./path.txt
# kill the previous scanSegments process
kill -9 $(pgrep -f uploadQueue)
# start the scanSegments process
nohup $rootPath/uploadQueue.sh > $rootPath/logs/uploadQueue.log 2>&1 &
# Check if the last command was successful
if [ $? -eq 0 ]; then
    echo "success"
else
    echo "An error occurred while starting uploadQueue. Check the logs for details."
fi