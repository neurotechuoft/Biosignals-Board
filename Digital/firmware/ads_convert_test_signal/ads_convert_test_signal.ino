// This code is a modified version of example_2.ino provided in the ADS1299 library.
// It will bootup the ADS and configure its control registers to use a test signal as input 
#include <ADS1299.h>

/*
  Arduino Uno Pin Assignments 
  SCK = 13
  MISO [DOUT] = 12
  MOSI [DIN] = 11
  CS = 10;
  RESET = 7;
  DRDY = 8;

*/

#include <ADS1299.h>

ADS1299 ADS;


void setup() {
  // don't put anything before the initialization routine for recommended POR
  ADS.initialize(8,7,10,4, false); // (DRDY pin, RST pin, CS pin, SCK frequency in MHz);

  Serial.begin(115200);
  Serial.println("ADS1299 Converting Test Signal");
  delay(1000);

  ADS.verbose = true;      // when verbose is true, there will be Serial feedback
  ADS.RESET();             // all registers set to default
  ADS.SDATAC();            // stop Read Data Continuous mode to communicate with ADS
 
}

void loop() {
  if (deviceIDReturned == false) {
    // Send SDATAC command before writing to registers since device wakes up in RDATAC mode
    ADS.SDATAC();
    
    // Configure ADS to use internal reference
    ADS.WREG(CONFIG3, 0xE0);
  
    // Set device data rate to fmod / 4096
    ADS.WREG(CONFIG1, 0x96);
    ADS.WREG(CONFIG2, 0xC0);

    // Set all channels to input short
    byte channel1_addr = 0x05;
    for (int ii = 0; ii < 8; ii++) {
      ADS.WREG(channel1_addr + ii, 0x01);
    }

    ADS.START();
    ADS.RDATAC();
    
    // Wait for DRDY to go high and look for noise (Issue 216 SCLKS)
    // SCLK is controlled by SPI library
    while (digitalRead(DRDY) == 0) {
    }

    ADS.SDATAC();   
    // Set all channels to mux in test signals
    ADS.WREG(CONFIG2, 0xD0);
    for (int jj = 0; jj < 8; jj++) {
      ADS.WREG(channel1_addr + jj, 0x05);
    }  
    ADS.RDATAC();  
	
    // Look for DRDY and Issue 216 SCLKS	
   }
}

