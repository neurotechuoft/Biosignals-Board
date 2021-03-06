// ads1299.c
// ADS1299 library
// Product Page - http://www.ti.com/product/ads1299
// Datasheet - http://www.ti.com/lit/ds/symlink/ads1299.pdf

#include <stdint.h>
#include <stdio.h>
#include "bcm2835.h"
#include "ads1299.h"
#include "ADS1299_definitions.h"

////////////// Global Variables //////////////

// Variable used to store the current data mode of the ADS1299.
// Either RDATAC, SDATAC, or RDATAC.
int ADS1299_current_data_mode;

int ADS1299_pga_gain;

//////////////////////////////////////////////

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
	    // BCM2835_SPI_MODE1 = 1,  // CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
    	bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   // The default
    
	    // Set SPI clock speed
	    // BCM2835_SPI_CLOCK_DIVIDER_512   = 512,     ///< 512 = 2.048us = 488.28125kHz
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
	// If the device is currently in RDATAC or RDATA mode, send SDATAC command to stop data transfer
	// and write to registers
	if (((ADS1299_current_data_mode == RDATAC_MODE) || (ADS1299_current_data_mode == RDATA_MODE)) &&
		((_cmd == _WREG) || (_cmd == _RREG))) {
		printf("\nWarning - Attempted RREG or WREG while reading data. Sending SDATAC\n");
		transferCmd(_SDATAC);
	}

	transferData(_cmd);

	if (_cmd == _SDATAC) {
		// Always wait for 4*TCLK after after _SDATAC passed
		bcm2835_delayMicroseconds(4*TCLK);
	}

	// Only do this if transfer was complete
	if ((_cmd == _RDATAC) || (_cmd == _SDATAC) || (_cmd == _RDATA)) {
		ADS1299_current_data_mode = _cmd;
		if        (ADS1299_current_data_mode == _SDATAC) {
			printf("Data mode set to SDATAC\n");
		} else if (ADS1299_current_data_mode == _RDATAC) {
			printf("Data mode set to RDATAC\n");
		} else {
			printf("Data mode set to RDATA\n");
		}	
	}

}


// function to get the device id of ads1299
// input: none
// return: device id in hex
// note: the device id should be 0x3e
uint8_t getDeviceId()
{
	uint8_t deviceid;

	// Calling _SDATAC for stopping continuous data mode
	transferCmd(_SDATAC);

	// Always wait for 4*TCLK after after _SDATAC passed
	bcm2835_delayMicroseconds(4*TCLK);

	// Calling _RREG for initializing read of registers
	transferCmd(_RREG);

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
	uint8_t opcode, cmd;
	uint8_t n_registers = 0; // Number of registers we're reading/writing - 1

	// If _readorwrite == 1, we are reading, otherwise, we are writing
	if (_readorwrite)
		opcode = _RREG; // 0x20
	else
		opcode = _WREG; // 0x40

	// Build command by adding the base address to the opcode 
	cmd = opcode + _rregadd;

	// RREG and WREG are two byte commands. Send opcode + address as the first byte
	transferCmd(cmd);

	// The second byte of the command is the number of registers to be read from the
	// base address (n - 1). Here, send 0 as the second byte to indicate we are
	// reading/writing one register.
	transferData(n_registers);

	// Send/receive data to/from the ADS1299 
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
