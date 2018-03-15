
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

void ADS1299_init() {

    delayMicroseconds(50000);

	// Configure ADS1299 - specific interface pins

	// Set the pin_DRDY to be the input
    bcm2835_gpio_fsel(PIN_DRDY, BCM2835_GPIO_FSEL_INPT);


	// Set the pin to output
	bcm2835_gpio_fsel(PIN_RESET, BCM2835_GPIO_FSEL_OUTP);

	// Set the pin to output
	bcm2835_gpio_fsel(PIN_PWDN, BCM2835_GPIO_FSEL_OUTP);

	// Configure test pins as outputs

	// FIXME: Uncomment this line. Currently commented out since TEST_PIN_1 (GPIO_J8_03) has been repurposed as PIN_RESET
	//bcm2835_gpio_fsel(TEST_PIN_1, BCM2835_GPIO_FSEL_OUTP);

	//bcm2835_gpio_fsel(TEST_PIN_2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TEST_PIN_3, BCM2835_GPIO_FSEL_OUTP);

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

    // Select the polarity LOW
    // ADS1299 uses an active low chip select
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

    // Note: The chip select pin is controlled automatically if using the bcm2835_spi_transfer() function.
    // Do not try writing to the pin or setting it as an OUTPUT!!

    // Initialize pins to default values

    // FIXME: UNcomment

    bcm2835_gpio_write(PIN_RESET, LOW);
    bcm2835_gpio_write(PIN_PWDN,  LOW);
    delayMicroseconds(50000);

    //bcm2835_gpio_write(PIN_DRDY,  LOW);

    // PIN_CS (chip select CE0) will be held HIGH when an SPI transfer is not in progress since we
    // set the chip select polarity to LOW.
    #ifdef __DEBUG__
    printf("After ADS1299 initialization\n");
    display_all_pin_states();
    printf("End of ADS1299_init()\n");
    #endif
}

void ADS1299_bootup(){
/* Function: Bootup Routine of the ADS1299
   Return: None */


    bcm2835_gpio_write(PIN_RESET, HIGH);

	// Bring PWDN pin high to exit low-power standby mode. Delay 100 us
	bcm2835_gpio_write(PIN_PWDN, HIGH);
	delayMicroseconds(pow(2, 18)  * TCLK);

	// Delay 2 seconds for VCAP1 >= 1.1 V
	delayMicroseconds(2000000);

	//Set the pin to low and delay for 2*TCLK us (tRST) as per datasheet
	bcm2835_gpio_write(PIN_RESET, LOW);
	delayMicroseconds(2 * TCLK);

	//set pin to high to reset and bootup; delay 16 TCLK after end of RESET pulse
	//or 18 TCLK after start of RESET pulse, as per Power Up Sequence in datasheet (pg. 70)
	bcm2835_gpio_write(PIN_RESET,HIGH);
	delayMicroseconds(16*TCLK);

	// Initialize some state variables

	// ADS1299 boots up into RDATAC mode
	ADS1299_current_data_mode = RDATAC_MODE;

	// The default PGA gain on bootup is 24
	ADS1299_pga_gain = 24;

    printf("After ADS1299 bootup\n");
    display_all_pin_states();
    printf("End of ADS1299_bootup()\n");

}

void ADS1299_config() {
	// We are using an internal reference, so set PD_REFBUF to 1
	printf("\nSetting PD_REFBUF bit of CONFIG3 to 1 \n");
	ADS1299_write_register(CONFIG3, 0xE0);

	// Arbritrary delay to wait for internal reference to settle
    bcm2835_delayMicroseconds(500);
}

void ADS1299_power_down() {
	printf("\n Powering down ADS1299...\n");
	bcm2835_gpio_write(PIN_PWDN, LOW);
    bcm2835_gpio_write(PIN_RESET, LOW);
	if (!bcm2835_gpio_lev(PIN_PWDN))
		printf("\n ADS1299 Powered down\n");
}

void ADS1299_reboot() {
	bool powered_on = false;

	powered_on = bcm2835_gpio_lev(PIN_PWDN);

	if (powered_on) {
		printf("\n ADS1299 is powered on. Rebooting ...\n");

		ADS1299_power_down();

		bcm2835_delayMicroseconds(2 * 1000000); // Wait for 2 seconds before powering up again

		ADS1299_init();
		ADS1299_bootup();

		printf("\n ADS1299 Reboot Finished\n");
	} else {
		printf("\n ADS1299 is already powered-down. Aborting reboot ...\n");
	}
}

//////////////////////// Register interface functions //////////////////////
/* Function: Read a single register of the ADS1299
   Return: The data read from the register */
uint8_t ADS1299_read_register(uint8_t reg_addr) {

	return rregTransferData(1, reg_addr, 0);
}

// Function: Read a field from a register.
// Arguments:
//	reg_addr     - The register base address
//	fld_size   - The size of the field in number of bits
//	fld_offset - The offset of the field from bit 0. Example if the LSB of the field is bit 3, field_offset is 3
uint8_t ADS1299_read_register_field(uint8_t reg_addr, uint8_t fld_size, uint8_t fld_offset) {
	uint8_t rd_data, rd_fld_data;
	uint8_t rd_msk;

	// Read the register over the SPI interface
	rd_data = ADS1299_read_register(reg_addr);

	// Create read mask by making making all bottom <fld_size> bits of the mask equal to 1.
	// For example:
	//		field_size = 3
	//		rd_msk 	   = 0b00000001 << 3 - 1
	//				   = 0xb0001000 - 1
	//				   = 0b00000111;
	rd_msk = ((1 << fld_size) - 1);

	// To get just the field data, discard bits lower than the field by shifting the LSB bit of the field
	// into bit 0. Then AND with the rd_msk to zero out bits higher than the MSB of the field.
	rd_fld_data = (rd_data >> fld_offset) & rd_msk;


		#ifdef __DEBUG__
	printf("\nADS1299_read_register_field - reg_addr: "BYTE_TO_BIN_PATTERN", fld_size: "BYTE_TO_BIN_PATTERN", fld_offset: "BYTE_TO_BIN_PATTERN", rd_data: "BYTE_TO_BIN_PATTERN", rd_fld_data: "BYTE_TO_BIN_PATTERN"\n",
			BYTE_TO_BIN(reg_addr),
			BYTE_TO_BIN(fld_size),
			BYTE_TO_BIN(fld_offset),
			BYTE_TO_BIN(rd_data),
			BYTE_TO_BIN(rd_fld_data));

	#endif

	return rd_fld_data;
}

// Function: Write a single register of the ADS1299
void ADS1299_write_register(uint8_t reg_addr, uint8_t reg_data) {
	rregTransferData(0, reg_addr, reg_data);
}

// Function: Write a field of a register.
// Arguments:
//	reg_addr     - The register base address
//	fld_size     - The size of the field in number of bits
//	fld_offset   - The offset of the field from bit 0. Example if the LSB of the field is bit 3, field_offset is 3
void ADS1299_write_register_field(uint8_t reg_addr, uint8_t fld_size, uint8_t fld_offset, uint8_t fld_data) {
	uint8_t wr_data, rd_data, msk_data;
	uint8_t wr_msk;

	// Create the write mask. This is to leave bits outside of the target field unmodified from the write.
	// Example:
	// 		Current register contents: 0b10101010
	//      fld_size   : 3
	//		fld_offset : 3
	//		Field data : 0b010
	//
	//		wr_mask = ~(((1 << 3) - 1) << 3)
	//				= ~((0b00001000 - 1) << 3)
	//				= ~(0b00000111 << 3)
	//    			= ~(0b00111000)
	//				= 0b11000111
	wr_msk = ~(((1 << fld_size) - 1) << fld_offset);

	// Read the register over the SPI interface
	rd_data = ADS1299_read_register(reg_addr);

	// Zero out only the bits in the field
	msk_data = rd_data & wr_msk;

	// Do a logical OR to write the field data into the zeroed field. The other bits of the register are untouched.
	wr_data = msk_data | (fld_data << fld_offset);

	ADS1299_write_register(reg_addr, wr_data);

	#ifdef __DEBUG__
	printf("\nADS1299_write_register_field - reg_addr: "BYTE_TO_BIN_PATTERN", fld_size: "BYTE_TO_BIN_PATTERN", fld_offset: "BYTE_TO_BIN_PATTERN", fld_data: "BYTE_TO_BIN_PATTERN", rd_data: "BYTE_TO_BIN_PATTERN", wr_data: "BYTE_TO_BIN_PATTERN"\n",
			BYTE_TO_BIN(reg_addr),
			BYTE_TO_BIN(fld_size),
			BYTE_TO_BIN(fld_offset),
			BYTE_TO_BIN(fld_data),
			BYTE_TO_BIN(rd_data),
			BYTE_TO_BIN(wr_data));
	#endif

}


bool register_check(uint8_t reg_addr, uint8_t expected, uint8_t * actual) {

	#ifdef __DEBUG__
	printf("\n-- Testing address 0x%x --\n", reg_addr);
	#endif

	*actual = ADS1299_read_register(reg_addr);

	if (*actual == expected) {
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

	printf("\n--- ADS1299 register defaults test ---\n");

	// Exempt Device ID check from result
	register_check(     ID,      ID_DEFAULT,      &reg_id);

	result &= register_check(CONFIG1, CONFIG1_DEFAULT, &reg_config1);

	result &= register_check(CONFIG2, CONFIG2_DEFAULT, &reg_config2);

	// FIXME: Might need to remove CONFIG3 from bootup test. We write to it before testing to enable the internal reference
	register_check(CONFIG3, CONFIG3_DEFAULT, &reg_config3);

	if (result)
		printf("\n--- ADS1299 register defaults test PASSED ---\n");
	else
		printf("\n--- ADS1299 register defaults test FAILED ---\n");

	return result;
}

bool ADS1299_test_registers_write() {

	uint8_t id_wr_data, config1_wr_data, config2_wr_data, config3_wr_data;
	uint8_t id_rd_data, config1_rd_data, config2_rd_data, config3_rd_data;

	bool result = true;

	id_wr_data = 0x0;
	config1_wr_data = 0x1;
	config2_wr_data = 0x2;
	config3_wr_data = 0x3;


	printf("\n--- ADS1299 register bash test ---\n");

	// Do not write to Device ID register
	//ADS1299_write_register(ID, id_wr_data);
	//result &= register_check(ID, id_wr_data, &id_rd_data);

	ADS1299_write_register(CONFIG1, config1_wr_data);
	result &= register_check(CONFIG1, config1_wr_data, &config1_rd_data);

	ADS1299_write_register(CONFIG2, config2_wr_data);
	result &= register_check(CONFIG2, config2_wr_data, &config2_rd_data);

	// FIXME: Might need to remove CONFIG3 from test, do not want to clobber PD_REFBUF bit
	//ADS1299_write_register(CONFIG3, config3_wr_data);
	//result &= register_check(CONFIG3, config3_wr_data, &config3_rd_data);

	if (result)
		printf("\n--- ADS1299 register write test PASSED ---\n");
	else
		printf("\n--- ADS1299 register write test FAILED ---\n");


	return result;
}



/////////////////////////////////////////////////////////////////////////////

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

void display_all_pin_states() {
	printf("\tAll Pin states:\n");
	display_pin_state(PIN_DRDY);
	display_pin_state(PIN_RESET);
	display_pin_state(PIN_PWDN);
	display_pin_state(PIN_MOSI);
	display_pin_state(PIN_MISO);
	display_pin_state(PIN_SCLK);
	display_pin_state(PIN_CS);
}

void display_pin_state(int _pin) {
	if (_pin == PIN_DRDY )
		printf("\tPIN_DRDY 			 : %x\n", bcm2835_gpio_lev(PIN_DRDY));
	else if (_pin == PIN_RESET)
		printf("\tPIN_RESET			 : %x\n", bcm2835_gpio_lev(PIN_RESET));
	else if (_pin == PIN_PWDN)
		printf("\tPIN_PWDN 			 : %x\n", bcm2835_gpio_lev(PIN_PWDN));
	else if (_pin == PIN_MOSI)
		printf("\tPIN_MOSI 			 : %x\n", bcm2835_gpio_lev(PIN_MOSI));
	else if (_pin == PIN_MISO)
		printf("\tPIN_MISO 			 : %x\n", bcm2835_gpio_lev(PIN_MISO));
	else if (_pin == PIN_SCLK)
	    	printf("\tPIN_SCLK 			 : %x\n", bcm2835_gpio_lev(PIN_SCLK));
	else if (_pin == PIN_CS)
		printf("\tPIN_CS   			 : %x\n", bcm2835_gpio_lev(PIN_CS));
	else
		printf("\tRPI_BPLUS_GPIO_J8_%d : %x\n", _pin, bcm2835_gpio_lev(_pin));
}







