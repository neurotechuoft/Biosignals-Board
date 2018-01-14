#!/bin/bash

LOG_DIR=$BSB_EMBEDDED/logs
CREATE_DATE=$(date +"%Y_%m_%d_%I%M%S")
LOG_NAME=bootup_$CREATE_DATE.log
LOG_FILE=$LOG_DIR/$LOG_NAME

printf "\nRunning ./bin/neuro ...\n"
sudo ./bin/neuro > $LOG_FILE

printf "Finished running ./bin/neuro\n"

# Point to log file location
printf "\nOutput in $LOG_FILE\n\n"

# Print bootup result (last line of log file)
tail -1 $LOG_FILE
