/*Developed by Joel Murphy and Conor Russomanno (Summer 2013)
  This example uses the ADS1299 Arduino Library, a software bridge between the ADS1299 chip and 
  Arduino. See http://www.ti.com/product/ads1299 for more information about the device and the README
  folder in the ADS1299 directory for more information about the library.
  
  This program reads all the ADS1299 registers, modifies CONFIG3, then modifies all 8 channel registers.
  Yup, it's a simple SPI link hello world functional test. When verbose is true, there will be Serial feedback.
  NOTE: the datasheet will tell you that the Channel Set Registers (0x05 to 0x0C) default to 0x60. Don't believe it.
  The default value for these locations is 0x61. 
*/

#include <ADS1299.h>

ADS1299 ADS;

/* 
 *  Arduino -> ADS1299 Pins
 *  -----------------------
 *  Pin                   (ADS)  (Arduino)      (ADS I/O)
 *  -------------------------------------------------------
 *  PIN_SCLK              (40) -> (D13)           (I)
 *  PIN_MOSI (ADS DOUT)   (43) -> (D12)           (O)
 *  PIN_MISO (ADS DIN)    (34) -> (D11)           (I)
 *  PIN_CS                (39) -> (D10)           (I)
 *  PIN_CLKSEL            (40) -> (D13)           (I)
 *  PIN_DRDY              (47) ->  (D8)           (O)
 *  PIN_RESET             (36) ->  (D7)           (I)
 *  PIN_PWDN              (35) ->  (D6)           (I)
 */

void setup() {
   pinMode(PIN_CLKSEL, OUTPUT);
   digitalWrite(PIN_CLKSEL, HIGH); // Set CLKSEL to high because using internal oscillator

  ADS.initialize(PIN_DRDY, PIN_RESET, PIN_CS, PIN_PWDN, 4, false); // (DRDY pin, RST pin, CS pin, SCK frequency in MHz, Daisy_en = 0);

  Serial.begin(115200);
  Serial.println("\n");
  Serial.println("ADS1299 Bootup"); 
  delay(1000);   

  ADS.verbose = true;      // when verbose is true, there will be Serial feedback
  
  ADS.RESET();             // all registers set to default
  ADS.SDATAC();            // stop Read Data Continuous mode to communicate with ADS  
  ADS.getDeviceID();

  ADS.RREGS(0x00,0x17);    // read ADS registers starting at 0x00 and ending at 0x17
  
  //----------------------------------------------------
  //----------------------------------------------------
  
  //ADS.WREG(CONFIG3,0xE0);  // enable internal reference buffer
  //ADS.RREG(CONFIG3);       // verify write
  //for(byte i=CH1SET; i<=CH8SET; i++){   // set up to modify the 8 channel setting registers
  //  ADS.regData[i] = 0x60;           // the regData array mirrors the ADS1299 register addresses
  //}                                  
  //ADS.WREGS(CH1SET,7);               // write new channel settings
  //ADS.RREGS(CH1SET,7);               // read out what we just did to verify the write
  //ADS.RDATAC();                      // enter Read Data Continuous mode
  
} // end of setup

void loop(){
  //Serial.println("-----------------------------------------------------\n");
  //delay(1000);
  //ADS.RREGS(0x00,0x17);    // read ADS registers starting at 0x00 and ending at 0x17
  //delay(1000);
} // end of loop
