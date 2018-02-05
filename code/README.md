# Repository Structure

## Setup

To standardize the working environment for all members during development, we use a bash [script](../../master/code/software/ADS_1299_Embedded/scripts/setup.sh) to setup useful environment variables that will be referenced by our Makefiles and other scripts. This script can also be called using the alias **setup_bsb** on our Raspberry Pi. That will setup all the variables and can be called from anywhere. Currently, the script will setup the following environment variables:
  - $BSB_HOME - The top directory for the Biosignals-Board Github repository. Note that the method used to find this directory is not perfect, it has only been tested on select systems. You may need to set this variable manually or modify the script.
  - $BSB_EMBEDDED = $BSB_HOME/code/software/ADS_1299_Embedded
  - $BSB_ANA = $BSB_HOME/code/analog
  - $BSB_GUI = $BSB_HOME/code/gui
  - $BSB_DIG = $BSB_HOME/code/digital
  - $BSB_PCB = $BSB_HOME/EAGLE


### [./analog](../../master/code/analog)
  - This directory contains Python programs written by the Analog Team. These programs were written for experimenting/learning, and are currently not part of the main code-base.
  
### [./digital](../../master/code/digital)
  - This directory contains much of the code and documentation from the early planning stages of the project. 
    - [**./doc/**](../../master/code/digital/doc)  - Contains ADS1299 datasheet, and all spreadsheets and documents outlining the our pin configuration and connections for each of the 64 pins. Summaries of ADS1299 registers as well as important equations and parameters are also documented in here.
    - [**./lib/**](../../master/code/digital/lib) - Contains the ADS1299 Arduino library, sourced from: https://github.com/OpenBCI/OpenBCI-V2hardware-DEPRECATED/tree/master/Arduino/Libraries/ADS1299. This code was used as a reference and for preliminary testing interfacing the ADS1299 chip with an Arduino. We stopped developing around this code when we switched to developing on the Raspberry Pi, in September 2017.
    - [**./src/**](../../bmaster/code/digital/src) - Contains two folders for two Arduino projects. **arduino_ads** is the code we ran to interface the Arduino with the ADS1299. **arduino_spi** contains code, one of our members, Musa, wrote to learn SPI communication.
    
### [./gui](../../master/code/gui)
  - This directory contains the code developed for the GUI.
  
### [./software](../../master/code/software)
  - This directory contains all code written by the Embedded Team post-September 2017.
    - [**./ADS_1299_Embedded/**](../../master/code/software/ADS_1299_Embedded) - Contains the main program for the Raspberry Pi to interface with the ADS1299. 
    - [**./arduino/**](../../master/code/software/arduino) - Contains an Arduino program to emulate the ADS1299 SPI command interface.
    - [**./hardware_tests/**](../../master/code/software/hardware_tests) - Raspberry Pi specific tests
    - [**./raspberrypi/**](../../master/code/software/raspberrypi) - Other code used with the Raspberry Pi
      
