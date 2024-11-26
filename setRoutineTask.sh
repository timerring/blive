#!/bin/bash

# Query the project path
CURRENT_PATH=$(pwd)
# Export the project path to the .bashrc file for the current user using the bash shell
echo "export BILIVE_PATH=$CURRENT_PATH # This for timerring/bilive project path" >> ~/.bashrc
# Make the changes to the .bashrc file immediately effective
echo " Have set the project path $CURRENT_PATH to .bashrc for the current user."