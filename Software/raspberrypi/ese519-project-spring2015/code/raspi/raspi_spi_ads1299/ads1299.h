//ads1299.h

#ifndef _ADS1299_h
#define _ADS1299_h


int initLibrary();
uint8_t transferData(uint8_t);
void transferComplete();
void transferCmd(uint8_t);
uint8_t getDeviceId();

#endif
