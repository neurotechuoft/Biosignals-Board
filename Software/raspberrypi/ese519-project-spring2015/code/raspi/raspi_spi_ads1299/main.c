// main.c
// This code is used to hack the ADS1299 development kit (http://www.ti.com/tool/ads1299eegfe-pdk) using a raspberry pi plus board.
// Written by Shanjit Singh Jajmann
// Other resources used - bcm2835 library used for Raspi BPlus pin access
// ESE 519 Project at the University of Pennsylvania

// Variable naming convention
// All functions are camelCase
// All variables are small case
// All macros are CAPS

// Pin Connections on hardware 
// MOSI - P19 on J8 on Pi B+ 
// MISO - P21 on J8 on Pi B+
// SS - P18 on J8 on Pi B+
// SCLK - P23 on J8 on Pi B+
// GND - P25 on J8 on Pi B+
// DRDY - Pin 22 on J8 on Pi B+
// clk config on the board on startup

#define RREG_READ 1
#define RREG_WRITE 0

// for printf
#include <stdio.h>

// for uint8_t datatypes
#include <stdint.h>

// for all pin definitions and constants
#include "definitions.h"

// for the custom ads1299 library
#include "ads1299.h"

// return codes - echo $?
// -1: library initialization failed
// -2: incorrect device id, check SPI
// 

int main(int argc, char **argv)
{
	// initialize library
	// resets the ads1299
	// sends the SDATAC command to stop continuos read mode
 	if(initLibrary())
        return -1;

	// variable to get/put spi data throughout this file
	uint8_t data;

	data = getDeviceId();

	// print device id
	printf("Device ID %02x\n", data);
	
	// exit if incorrect device id
	if (data!=0x3e)
	{
		printf("Incorrect Device ID detected, Exiting\n");
		return -2;
	}

	// print all configuration registers 
	





	transferComplete();
	
	return 0;
}



