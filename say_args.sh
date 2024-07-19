#!/bin/bash

# check if the parameter is provided
if [ $# -eq 0 ]; then
    echo "no parameter"
    exit 1
fi

# Read each parameter and output
for arg in "$@"
do
    echo "parameter: $arg"
done