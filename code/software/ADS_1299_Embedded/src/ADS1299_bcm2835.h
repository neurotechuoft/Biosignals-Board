#ifndef ADS1299_bcm2835
#define ADS1299_bcm2835

void ADS1299_init();

void ADS1299_bootup();

///////////// SPI Interface functions //////////////

void ADS1299_read_register(uint8_t reg_addr);


/////////////// Testing functions //////////////////

void ADS1299_test_registers();

/////////////// Other functions ///////////////////

uint8_t ADS1299_read_register_field(uint8_t reg_data, uint8_t lsb_pos, uint8_t msb_pos);

#endif
