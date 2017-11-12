#include "bcm2835.h"
#include <ADS1299_bcm2835.h>
#include <ADS1299_definitions.h>

// Include ese519 library
#include <ads1299.h>
 
void ADS1299_init() {

	// Configure ADS1299 - specific interface pins

	// Set the pin_DRDY to be the input 
    bcm2835_gpio_fsel(PIN_DRDY, BCM2835_GPIO_FSEL_INPT);


	// Set the pin to output
	bcm2835_gpio_fsel(PIN_RESET, BCM2835_GPIO_FSEL_OUTP);

	// Configure BCM2835 SPI settings

	// Begin spi by initializing all the required pins
	bcm2835_spi_begin();

	// Set bit order
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    
    // Set the spi mode - 4 modes
	//	BCM2835_SPI_MODE1 = 1,  // CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE1);                   // The default
    
	//Set SPI clock speed
	//	BCM2835_SPI_CLOCK_DIVIDER_512   = 512,     ///< 512 = 2.048us = 488.28125kHz
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_512); // The default

    // Chip select
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    
    // Select the polarity
    // LOW or HIGH
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, HIGH);

    // Set the ss pin to output
    bcm2835_gpio_fsel(PIN_CS, BCM2835_GPIO_FSEL_OUTP);
}

void ADS1299_bootup(){
/* Function: Bootup Routine of the ADS1299
   Return: None */

	//Set the pin to low and delay (TIME TO BE DECIDED BASED ON TESTING)
	bcm2835_gpio_write(PIN_RESET, LOW);
	delay(10);

	//set pin to high to reset and bootup; delay (TIME TO BE DECIDED DURING TESTING)
	bcm2835_gpio_write(PIN_RESET,HIGH);
	delay(100);

}

/* Function: Read a single register of the ADS1299
   Return: The data read from the register */
uint8_t ADS1299_read_register(uint8_t reg_addr) {
	return rregTransferData(1, reg_addr, 0);
}

void ADS1299_test_registers() {

	// Device powers-up in RDATAC mode, so send SDATAC command to stop data transfer and write to registers
	transferCmd(_SDATAC);

	// Always wait for 4*TCLK after after _SDATAC passed
	bcm2835_delayMicroseconds(4*TCLK);

	// Read all control registers
	uint8_t reg_id, reg_config1, reg_config2, reg_config3;

	reg_id      = ADS1299_read_register(ID);
	reg_config1 = ADS1299_read_register(CONFIG1);
	reg_config2 = ADS1299_read_register(CONFIG2);
	reg_config3	= ADS1299_read_register(CONFIG3);
}





