#!/bin/bash
# DESCRIPTION:
#   Delete the log files in a certain frequency.
#   You can set it as a timing task via the `chmod +x` first, then `crontab -e`.
#   0 0 */3 * * cd /path/to/thisFolder && ./script.sh
#
# PARAMETERS:
#   INPUT:  none.
#   OUTPUT: none.

targetDir="$BILIVE_PATH/logs"

# Retrieve seconds
currentTime=$(date +%s)

# Recursively traverse all files in the target folder and its subfolders.
find "$targetDir" -type f | while read file; do
    # Retrieve the file modified time
    fileTime=$(stat -c %Y "$file")
    # Calculate the time gap
    timeDiff=$((currentTime - fileTime))
    # Check if the gap is greater than x days(x * 24 * 60 * 60 seconds). You can modify it.
    if [ $timeDiff -ge $((3 * 24 * 60 * 60)) ]; then
        rm -f "$file"
    fi
done