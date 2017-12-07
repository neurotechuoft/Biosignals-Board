
// for printf
#include <stdio.h>

// for uint8_t datatypes
#include <stdint.h>
#include <stdbool.h>

#include <math.h>

#include "ADS1299_definitions.h"
#include "bcm2835.h"
#include "ads1299.h"
#include "ADS1299_bcm2835.h"
#include "zhash.h"

void ADS1299_init() {

	// Configure ADS1299 - specific interface pins

	// Set the pin_DRDY to be the input 
    bcm2835_gpio_fsel(PIN_DRDY, BCM2835_GPIO_FSEL_INPT);


	// Set the pin to output
	bcm2835_gpio_fsel(PIN_RESET, BCM2835_GPIO_FSEL_OUTP);

	// Set the pin to output
	bcm2835_gpio_fsel(PIN_PWDN, BCM2835_GPIO_FSEL_OUTP);

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

    // Initialize pins to default values
    bcm2835_gpio_write(PIN_RESET, HIGH);
    bcm2835_gpio_write(PIN_PWDN,  LOW);
    bcm2835_gpio_write(PIN_CS,    LOW);   
}

void ADS1299_bootup(){
/* Function: Bootup Routine of the ADS1299
   Return: None */
	
	// Bring PWDN pin high to exit low-power standby mode. Delay 100 us
	bcm2835_gpio_write(PIN_PWDN, HIGH);
	delayMicroseconds(100);

	//Set the pin to low and delay for 2*TCLK us (tRST) as per datasheet
	bcm2835_gpio_write(PIN_RESET, LOW);
	delayMicroseconds(2*TCLK);

	//set pin to high to reset and bootup; delay (TIME TO BE DECIDED DURING TESTING)
	bcm2835_gpio_write(PIN_RESET,HIGH);
	delayMicroseconds(100);

	// Initialize some state variables

	// ADS1299 boots up into RDATAC mode
	ADS1299_current_data_mode = RDATAC_MODE;

	// The default PGA gain on bootup is 24
	ADS1299_pga_gain = 24;
}

/* Function: Read a single register of the ADS1299
   Return: The data read from the register */
uint8_t ADS1299_read_register(uint8_t reg_addr) {

	return rregTransferData(1, reg_addr, 0);
}

bool register_check(uint8_t reg_addr, uint8_t expected, uint8_t * actual) {

	#ifdef __DEBUG__
	printf("\n-- Testing address 0x%x --\n", reg_addr);
	#endif

	*actual = ADS1299_read_register(reg_addr);

	if (*actual && expected) {
		#ifdef __DEBUG__
		printf("--PASSED--");
		printf("\nRead: 0x%x  Expected: 0x%x\n", *actual, expected);
		#endif
		return true;
	} else {
		#ifdef __DEBUG__
		printf("--FAILED--");
		printf("\nRead: 0x%x  Expected: 0x%x\n", *actual, expected);
		#endif
		return false;
	}
}

bool ADS1299_test_registers() {

	bool result = true;

	// Always wait for 4*TCLK after after _SDATAC passed
	bcm2835_delayMicroseconds(4*TCLK);

	// Read all control registers
	uint8_t reg_id, reg_config1, reg_config2, reg_config3;

	// Exempt Device ID check from result
	register_check(     ID,      ID_DEFAULT,      &reg_id);

	/*Instantiate the HashMap*/
	struct ZHashTable *hash_table;
	hash_table = zcreate_hash_table();

	result &= register_check(CONFIG1, CONFIG1_DEFAULT, &reg_config1);
	result &= register_check(CONFIG2, CONFIG2_DEFAULT, &reg_config2);
	result &= register_check(CONFIG3, CONFIG3_DEFAULT, &reg_config3);

	
	/*Hash Everything*/
	char reg_str[4][128];
	sprintf(reg_str[0], "%d", reg_id);
	sprintf(reg_str[1], "%d", reg_config1);
	sprintf(reg_str[2], "%d", reg_config2);
	sprintf(reg_str[3], "%d", reg_config3);
	zhash_set(hash_table, "ID", (void *)reg_str[0]);
	zhash_set(hash_table, "CONFIG1", (void *)reg_str[1]);
	zhash_set(hash_table, "CONFIG2", (void *)reg_str[2]);
	zhash_set(hash_table, "CONFIG3", (void *)reg_str[3]);

	/*Check and print the Config-Register values*/
	if (zhash_exists(hash_table, "ID")) {
		printf("The device ID is  %s\n", (char *) zhash_get(hash_table, "ID"));
	}
	if (zhash_exists(hash_table, "CONFIG1")) {
		printf("Value of Config_Register1 is  %s\n", (char *) zhash_get(hash_table, "CONFIG1"));
	}
	if (zhash_exists(hash_table, "CONFIG2")) {
		printf("Value of Config_Register2 is  %s\n", (char *) zhash_get(hash_table, "CONFIG2"));
	}
	if (zhash_exists(hash_table, "CONFIG3")) {
		printf("Value of Config_Register3 is  %s\n", (char *) zhash_get(hash_table, "CONFIG3"));
	}


	/*Free Table */ //---> We need to move this to terminating function
	zfree_hash_table(hash_table);

	return result;
}

// Function pulses a pin _pin at _frequency (in Hz) for _test_duration seconds
void output_square_wave(int _pin, double _frequency, double _test_duration) {
    double test_wave_frequency = _frequency;
    double test_wave_period_us = (1/test_wave_frequency) * 1000000;
   
    int elapsed_cycles;
    int num_cycles_for_test = _test_duration * _frequency; 

    for (elapsed_cycles = 0; elapsed_cycles < num_cycles_for_test; elapsed_cycles++) {
	bcm2835_gpio_write(_pin, HIGH);
	delayMicroseconds(test_wave_period_us);

	bcm2835_gpio_write(_pin, LOW);
	delayMicroseconds(test_wave_period_us);
    }


}

double convert_reading_to_voltage(int ads1299_reading, int gain) {
	double full_scale, decoded_data, LSB;

	// According to page 8 of the datasheet
	full_scale = 2 * VREF / gain;

	// Convert 24 bit two's complement to 32 bit two's complement
	if ((ads1299_reading >> 23) & 0x1) {
		ads1299_reading |= 0xFF000000; // Pad upper 8 bits with ones
	} else {
		ads1299_reading &= 0x00FFFFFF;
	}

	LSB = (full_scale / (pow(2, ADC_RES - 1)));

	decoded_data = ads1299_reading * LSB;

	#ifdef __DEBUG__
		printf("full_scale = %.9g\nLSB = %.9g\nEncoded: 0x%x Decoded: %.9g\n\n", full_scale, LSB, ads1299_reading, decoded_data);
	#endif

	return decoded_data;
}

void test_convert_reading_to_voltage() {
	int i;

    for (i = -(pow(2,10)); i <= (pow(2,10)); i++) {
        convert_reading_to_voltage(i, 24);
    }
}





