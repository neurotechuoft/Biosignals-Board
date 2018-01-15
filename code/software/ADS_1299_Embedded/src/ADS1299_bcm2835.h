#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef ADS1299_bcm2835
#define ADS1299_bcm2835


void ADS1299_init();

void ADS1299_bootup();

///////////// SPI Interface functions //////////////

uint8_t ADS1299_read_register(uint8_t reg_addr);

/////////////// Testing functions //////////////////

bool ADS1299_test_registers();
bool register_check(uint8_t reg_addr, uint8_t expected, uint8_t * actual);

void output_square_wave(int _pin, double _frequency, double _test_duration);

/////////////// Other functions ///////////////////

uint8_t ADS1299_read_register_field(uint8_t reg_data, uint8_t lsb_pos, uint8_t msb_pos);
double convert_reading_to_voltage(int ads1299_reading, int gain);


// Tests
void test_convert_reading_to_voltage();
void display_pin_state(int _pin);
void display_all_pin_states();
#endif
