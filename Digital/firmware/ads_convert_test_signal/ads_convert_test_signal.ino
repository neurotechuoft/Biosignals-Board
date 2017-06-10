// This code is a modified version of example_2.ino provided in the ADS1299 library.
// It will bootup the ADS and configure its control registers to use a test signal as input 
#include <ADS1299.h>

ADS1299 ADS;

//Arduino Uno - Pin Assignments; Need to use ICSP for later AVR boards
// SCK = 13
// MISO [DOUT] = 12
// MOSI [DIN] = 11
// CS = 10; 
// DRDY = 9;

//  0x## -> Arduino Hexadecimal Format
//  0b## -> Arduino Binary Format

boolean deviceIDReturned = false;
boolean startedLogging = false;
int DRDY = 9;
int CS = 10;

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println("ADS1299-bridge has started!");
  

  
  ADS.setup(DRDY, CS); // (DRDY pin, CS pin);
  delay(10);  //delay to ensure connection
  
  ADS.RESET();
  
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

