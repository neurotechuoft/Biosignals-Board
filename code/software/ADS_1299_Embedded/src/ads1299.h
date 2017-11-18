//ads1299.h

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "bcm2835.h"

#ifndef _ADS1299_h
#define _ADS1299_h

int initLibrary();
uint8_t transferData(uint8_t);
void transferComplete();
void transferCmd(uint8_t);
uint8_t getDeviceId();
uint8_t rregTransferData(int _readorwrite, uint8_t _rregadd, uint8_t _rregvalue);

#endif
