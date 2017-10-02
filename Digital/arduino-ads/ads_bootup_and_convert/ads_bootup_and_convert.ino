//Developed by Conor Russomanno (June 2013)
//This example uses the ADS1299 Arduino Library, a software bridge between the ADS1299 TI chip and 
//Arduino. See http://www.ti.com/product/ads1299 for more information about the device and the reference
//folder in the ADS1299 directory for more information about the library.

//Note: this Library and example file are still in early stages of development.
/*Currently, you are able to:
  - execute all System Commands (Datasheet, pg. 35-36)
  - Enable/disable read data continuous mode (Datasheet, pg. 35-38)
  - Read and write registers of the ADS1299 using RREG and WREG with HEX or BINARY values to edit the appropriate bits (Datasheet, pg. 39-47)
  
  Future Methods:
  - Commands/functions to edit ADS1299 settings without having to rewrite entire register bytes (ex. "setNumChannels(4);" would automatically power-down channels 5-8 by accessing those bits behind the scenes)
  - Fix timing issue with RDATAC - right now some data is getting lost (from test below, you'll see the serial monitor consistantly prints ~1511 samples from 225ms to 10000ms, but that # should be ~2443 based on default 250samples/second setting)
 - add SD-card functionality to log data packets to an SD
  - add txt file creation/writing for data storage when operating through a computer
*/
 #include <ADS1299.h>

ADS1299 ADS;

// Arduino Pins
int pinSCLK = 13;
int pinMISO = 12;
int pinMOSI = 11;
int pinCS = 10;
int pinRESET = 7;
int pinDRDY = 8;

void setup() {
  // don't put anything before the initialization routine for recommended POR  
  ADS.initialize(pinDRDY, pinRESET, pinCS, 4, false); // (DRDY pin, RST pin, CS pin, SCK frequency in MHz, Daisy_en = 0);

  Serial.begin(115200);
  Serial.println("ADS1299 Bootup and Convert"); 
  delay(1000);   
  
  ADS.RESET();
}

void loop(){
  
  if(deviceIDReturned == false){
    
    ADS.getDeviceID(); //Function to return Device ID
    
    //prints dashed line to separate serial print sections
    Serial.println("----------------------------------------------");
    
    //Read ADS1299 Register at address 0x00 (see Datasheet pg. 35 for more info on SPI commands)
    ADS.RREG(0x00);
    Serial.println("----------------------------------------------");
    
    //PRINT ALL REGISTERS... Read 0x17 addresses starting from address 0x00 (these numbers can be replaced by binary or integer values)
    ADS.RREG(0x00, 0x17);
    Serial.println("----------------------------------------------");
    
    //Write register command (see Datasheet pg. 38 for more info about WREG)
    // Overwrites the CONFIG1 Register to:
    //     Enable multiple readback mode
    //     Connect interanal oscillator clock to CLK pin when CLKSEL pin = 1
    //     Set data rate to fMOD / 4096 (250 samples per second)
    //ADS.WREG(CONFIG1, 0b11010110);
    //Serial.println("----------------------------------------------");
    /
    //Repeat PRINT ALL REGISTERS to verify that WREG changed the CONFIG1 register
    //ADS.RREG(0x00, 0x17);
    //Serial.println("----------------------------------------------");
    
    //Start data conversions command
    //ADS.START(); //must start before reading data continuous
    deviceIDReturned = true;
  }
  
  //print data to the serial console for only the 1st 10seconds of 
  while(millis()<10000){
    if(startedLogging == false){
      Serial.print("Millis: "); //this is to see at what time the data starts printing to check for timing accuracy (default sample rate is 250 sample/second)
      Serial.println(millis());
      startedLogging = true;
    }
    
    //Print Read Data Continuous (RDATAC) to Ardiuno serial monitor... 
    //The timing of this method is not perfect yet. Some data is getting lost 
    //and I believe its due to the serial monitor taking too much time to print data and not being ready to recieve to packets
    ADS.updateData();  
  }
  
  
}
