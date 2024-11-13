#!/bin/bash
# DESCRIPTION:
#   Delete the log files in a certain frequency.
#   You can set it as a timing task via the `chmod +x` first, then `crontab -e`.
#   0 0 */3 * * cd /path/to/thisFolder && ./script.sh
#
# PARAMETERS:
#   INPUT:  none.
#   OUTPUT: none.
while read key value; do
    export $key="$value"
done < ./path.txt
target_dir="$root_path/logs"

# Retrieve seconds
current_time=$(date +%s)

# Recursively traverse all files in the target folder and its subfolders.
find "$target_dir" -type f | while read file; do
    # Retrieve the file modified time
    file_mtime=$(stat -c %Y "$file")
    # Calculate the time gap
    time_diff=$((current_time - file_mtime))
    # Check if the gap is greater than x days(x * 24 * 60 * 60 seconds). You can modify it.
    if [ $time_diff -ge $((5 * 24 * 60 * 60)) ]; then
        rm -f "$file"
    fi
done