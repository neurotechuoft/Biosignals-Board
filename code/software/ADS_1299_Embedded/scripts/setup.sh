#!/bin/bash

CURR_DIR=`pwd`

export BSB_HOME=`find $HOME/* -name "Biosignals-Board" | tail -1`

cd $BSB_HOME

export BSB_EMBEDDED="$BSB_HOME/code/software/ADS_1299_Embedded"
export BSB_ANA="$BSB_HOME/code/analog"
export BSB_GUI="$BSB_HOME/code/gui"
export BSB_DIG="$BSB_HOME/code/digital"
export BSB_PCB="$BSB_HOME/EAGLE"

#printf "Biosignals-Board Embedded Setup Complete\n\n"

cd $CURR_DIR

