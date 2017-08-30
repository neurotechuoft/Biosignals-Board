// ads1299.c
// ADS1299 library
// Product Page - http://www.ti.com/product/ads1299
// Datasheet - http://www.ti.com/lit/ds/symlink/ads1299.pdf


#include "bcm2835.h"
#include <stdint.h>
#include "definitions.h"
#include <stdio.h>
#include "ads1299.h"


// function call to set up all the communication between the raspberry pi bplus and the ADS1299 development board.
// input: none
// returns: 0 if everything inorder, else error code
// note: make sure spi settings are the same
// note: this function also resets the ads1299
// note: this function also sends SDATAC to stop continuous read
int initLibrary()
{
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
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    
    // Select the polarity
    // LOW or HIGH
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, HIGH);

    // Set the pin_DRDY to be the input 
    bcm2835_gpio_fsel(PIN_DRDY, BCM2835_GPIO_FSEL_INPT);

    // Set the ss pin to output
    bcm2835_gpio_fsel(PIN_CS, BCM2835_GPIO_FSEL_OUTP);
    
    // Set the ss pin to be high by default
    bcm2835_gpio_write(PIN_CS, HIGH);

	// --- RESET THE ADS1299 --- //
	// Issue the ss pulse
	// bcm2835_gpio_write(PIN_CS, LOW);
    // wait for a minimum of 2*TCLK    
    // bcm2835_delayMicroseconds(3*TCLK);
	// Make ss go high again
	// NOTE : Use the reset opcode (0x60) instead

    bcm2835_gpio_write(PIN_CS, LOW);
	bcm2835_delayMicroseconds(1000);

	// reset opcode sent
	transferData(0x60);
    
    // wait for min 18*TCLK for reset
	bcm2835_delayMicroseconds(20*TCLK);

	return 0;
}

// function to send/receive data after initialization
// input: data to be sent on MOSI line
// returns: data on MISO line
// note: prints mosi, miso when DEBUG 1
uint8_t transferData(uint8_t _data)
{	
	#ifdef __DEBUG__
		printf("MOSI: %02x \n", _data);
	#endif
	
	uint8_t recv = bcm2835_spi_transfer(_data);

	#ifdef __DEBUG__
		printf("MISO: %02x \n", recv);
	#endif

	return (recv);	
}

// function to send/receive data on spi for _nregs-1 number of registers
// input: 
// output: 
/*uint8_t transferNData(uint8_t _data, int _nregs)
{
	uint8_t *data
	for(i=0;i<_nregs;i++)
	{

	}

}*/

// function to transfer commands to the ads1299 - pg 35 of the datasheet
// input: none
// return: none
void transferCmd(uint8_t _cmd)
{
	// just send _RESET 
	transferData(_cmd);

}


// function to get the device id of ads1299
// input: none
// return: device id in hex
// note: the device id should be 0x3e
uint8_t getDeviceId()
{
	uint8_t deviceid;

	// Calling _SDATAC for stopping continuous data mode
	transferData(_SDATAC);

	// Always wait for 4*TCLK after after _SDATAC passed
	bcm2835_delayMicroseconds(4*TCLK);

	// Calling _RREG for initializing read of registers
	transferData(_RREG);

	// Number of registers to be read 1
	transferData(0x00);

	// push in dummy bit to get device id
	deviceid = transferData(0x00);

	// restart the _RDATAC mode
	// transferData(_RDATAC);

	return deviceid;
}

// function to directly read/write to a register 
// input: to read/write, register address, register value to write
// output: the read data in case of read and 0x00 in case of a write
uint8_t rregTransferData(int _readorwrite, uint8_t _rregadd, uint8_t _rregvalue)
{
	uint8_t data;

	// read is 1, write is 0
	if (_readorwrite)
		data = 0x20;
	else
		data = 0x40;

	data = data + _rregadd;

	transferData(data);
	transferData(0x00);

	if (_readorwrite)
		data = transferData(0x00);
	else
		transferData(_rregvalue);

	return data;
}

void transferComplete()
{
    bcm2835_spi_end();
    bcm2835_close();
}
