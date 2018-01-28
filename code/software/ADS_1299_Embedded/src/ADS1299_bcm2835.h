#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef ADS1299_bcm2835
#define ADS1299_bcm2835


void ADS1299_init();

void ADS1299_bootup();

void ADS1299_config();
///////////// Register Interface functions //////////////

uint8_t ADS1299_read_register(uint8_t reg_addr);
uint8_t ADS1299_read_register_field(uint8_t reg_addr, uint8_t fld_size, uint8_t fld_offset);
void ADS1299_write_register(uint8_t reg_addr, uint8_t reg_data);
void ADS1299_write_register_field(uint8_t reg_addr, uint8_t fld_size, uint8_t fld_offset, uint8_t fld_data);


/////////////// Testing functions //////////////////

bool ADS1299_test_registers();
bool ADS1299_test_registers_write();
bool register_check(uint8_t reg_addr, uint8_t expected, uint8_t * actual);

void output_square_wave(int _pin, double _frequency, double _test_duration);

/////////////// Other functions ///////////////////

double convert_reading_to_voltage(int ads1299_reading, int gain);


// Tests
void test_convert_reading_to_voltage();
void display_pin_state(int _pin);
void display_all_pin_states();
#endif
