/*
 * This program is a place to experiment with the Serial Plotter and Arduino SPI
 */
#include <ADS1299.h>

ADS1299 ADS;

// Arduino Pins
int pinSCLK = 13; 
int pinMISO = 12;
int pinMOSI = 11; 
int pinCS = 10; 
int pinCLKSEL = 9; 
int pinDRDY = 8;  
int pinRESET = 7; 
int pinPWDN = 6; 

void setup() {
   Serial.begin(115200);
   
   // ----------------- Setup ADS1299 ---------------------
   pinMode(pinCLKSEL, OUTPUT);
   digitalWrite(pinCLKSEL, HIGH); // Set CLKSEL to high because using internal oscillator

  // (DRDY pin, RST pin, CS pin, PWDN pin, SCK frequency in MHz, Daisy_en = 0);
  ADS.initialize(pinDRDY, pinRESET, pinCS, pinPWDN, 4, false); // (DRDY pin, RST pin, CS pin, SCK frequency in MHz, Daisy_en = 0);

  // -------------------- Done setup -------------------------
    
  ADS.setPlotMOSI(true);

  // Clear plot 
  if (ADS.getPlotMOSI() || ADS.getPlotMISO()) {
    for (int i = 0; i < 500; i++) {
      Serial.println(0);
    }
  }

  // Set marker for start of data we want to monitor (temporary solution)
  //Serial.println(1);
  //Serial.println(0);
  //Serial.println(1);
  //Serial.println(0);
  
  ADS.RESET(); // _RESET = 0x06 = 0000 0110
  delay(1000);
  ADS.SDATAC(); // _SDATAC = 0x11 = 0001 0001
  delay(1000);
  //ADS.transfer(0x55); // 0x55 = 0101 0101
} // end of setup

void loop(){
} // end of loop
