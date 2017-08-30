// ADS1299 library
// Product Page - http://www.ti.com/product/ads1299
// Datasheet - http://www.ti.com/lit/ds/symlink/ads1299.pdf
// Written by Shanjit Singh Jajmann

// Contributions 
// bcm2835 library used for Raspi BPlus pin access

// This is the template of the initial code which actually ran when compiled on the raspi b plus


// Variable naming convention
// All functionss are camelCase
// All variables are first letter capital: InitLibrary

//#include "ads1299.h"
#include <stdio.h>
#include <stdint.h>
#include "definitions.h"
#include "bcm2835.h"

// fclk = 2.048 Mhz, TCLK = 488.2815 nanoseconds	
#define TCLK 0.4882815 // microseconds
#define FCLK 2.048

uint8_t transferData(uint8_t data)
{	

//	bcm2835_gpio_write(PIN_RESET,LOW);
	printf("MOSI: %02x \n", data);
	uint8_t recv = bcm2835_spi_transfer(data);

//	bcm2835_gpio_write(PIN_RESET,HIGH);
	// not required but trying to put a delay for any effect
	//bcm2835_delayMicroseconds(1000); // 1millisecond

	printf("MISO: %02x \n", recv);
	return (recv);	
}


int main(int argc, char **argv)
{

	// Initialize the library 
	// Pin Numbers 
	// MOSI - P19 on J8 on Pi B+ 
	// MISO - P21 on J8 on Pi B+
	// SS - P24 on J8 on Pi B+
	// SCLK - P23 on J8 on Pi B+
	// GND - P25 on J8 on Pi B+
	// Note : The CS is automatically managed by the library


	// Reset - Pin 18 on J8 on Pi B+
	// DRDY - Pin 22 on J8 on Pi B+

	// CLKSEL pin is set to low
	// wait for internal oscillator to start up


	if (!bcm2835_init())
	return 24;

	// Begin spi by initializing all the required pins
	bcm2835_spi_begin();

	// Set bit order
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    
    // Set the spi mode - 4 modes
	//	BCM2835_SPI_MODE0 = 0,  // CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
	//	BCM2835_SPI_MODE1 = 1,  // CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
	//	BCM2835_SPI_MODE2 = 2,  // CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
	//	BCM2835_SPI_MODE3 = 3,  // CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   // The default
    
	//Set SPI clock speed
	//	BCM2835_SPI_CLOCK_DIVIDER_65536 = 0,       ///< 65536 = 262.144us = 3.814697260kHz (total H+L clock period) 
	//	BCM2835_SPI_CLOCK_DIVIDER_32768 = 32768,   ///< 32768 = 131.072us = 7.629394531kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_16384 = 16384,   ///< 16384 = 65.536us = 15.25878906kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_8192  = 8192,    ///< 8192 = 32.768us = 30/51757813kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_4096  = 4096,    ///< 4096 = 16.384us = 61.03515625kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_2048  = 2048,    ///< 2048 = 8.192us = 122.0703125kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_1024  = 1024,    ///< 1024 = 4.096us = 244.140625kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_512   = 512,     ///< 512 = 2.048us = 488.28125kHz
	//	BCM2835_SPI_CLOCK_DIVIDER_256   = 256,     ///< 256 = 1.024us = 976.5625MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_128   = 128,     ///< 128 = 512ns = = 1.953125MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_64    = 64,      ///< 64 = 256ns = 3.90625MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_32    = 32,      ///< 32 = 128ns = 7.8125MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_16    = 16,      ///< 16 = 64ns = 15.625MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_8     = 8,       ///< 8 = 32ns = 31.25MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_4     = 4,       ///< 4 = 16ns = 62.5MHz
	//	BCM2835_SPI_CLOCK_DIVIDER_2     = 2,       ///< 2 = 8ns = 125MHz, fastest you can get
	//	BCM2835_SPI_CLOCK_DIVIDER_1     = 1,       ///< 1 = 262.144us = 3.814697260kHz, same as 0/65536
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_512); // The default

    // Chip select
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
    
    // Select the polarity
    // LOW or HIGH
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, HIGH);      // the default

    // Set the pin_DRDY to be the input 
    bcm2835_gpio_fsel(PIN_DRDY, BCM2835_GPIO_FSEL_INPT);

    // Set the reset pin to the output
    bcm2835_gpio_fsel(PIN_RESET, BCM2835_GPIO_FSEL_OUTP);
    
    // Set the reset pin to be high by default
    bcm2835_gpio_write(PIN_RESET, HIGH);
	// wait for minimum one second for power on reset bootup
	bcm2835_delayMicroseconds(1500000);
	// --- RESET THE ADS1299 --- //
	// Issue the reset pulse
	//bcm2835_gpio_write(PIN_RESET, LOW);
    // wait for a minimum of 2*TCLK    
    //bcm2835_delayMicroseconds(3*TCLK);
	// Make reset go high again
    bcm2835_gpio_write(PIN_RESET, LOW);
	bcm2835_delayMicroseconds(1000);
	// reset opcode sent
	transferData(0x60);
    // wait for min 18*TCLK for reset 
	bcm2835_delayMicroseconds(20*TCLK);
	// variable to read data
	uint8_t readData;
	// stop the continuous read mode
       transferData(0x11);
	bcm2835_delayMicroseconds(4*TCLK);
    //  Get the device id registers from 0x00h
     uint8_t  writeData;
    writeData  = 0x20 + 0x00;
    transferData(writeData);
    // Get only one register
    transferData(0x00);
    // shift in dummy bits to read the data
    readData = transferData(0x00);
    printf("Device id is %02x\n", readData);
	transferData(0x21);
	transferData(0x00);
	readData = transferData(0x00);
//     readData = transferData(0x00);
	printf("config register is %02x \n", readData);


	// write to gpio pin 1 

	transferData(0x4e);
	transferData(0x00);

	transferData(0x10);

	transferData(0x4e);
	transferData(0x00);
	transferData(0xF0);

	transferData(0x2e);
	transferData(0x00);
	readData = transferData(0x00);

	bcm2835_delayMicroseconds(1000);
	bcm2835_gpio_write(PIN_RESET,HIGH);

    




	bcm2835_spi_end();
    bcm2835_close();

	return 0;
}


/*

	blinky code - use for debug later

    // Set the pin to be an output
    bcm2835_gpio_fsel(PIN_DRDY, BCM2835_GPIO_FSEL_OUTP);

	// Turn it on
    bcm2835_gpio_write(PIN, HIGH);
        
    // wait a bit
    bcm2835_delay(500);
        
    // turn it off
    bcm2835_gpio_write(PIN, LOW);
        
    // wait a bit
    bcm2835_delay(500);
	
*/
