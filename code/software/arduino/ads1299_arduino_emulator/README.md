## ADS1299 Arduino Emulator

This Arduino program emulates the ADS1299 SPI Interface to facilitate testing of our code without requiring access to the ADS1299 chip itself.
The program will be coded to reinforce the ADS1299 specifications as per its [datasheet](https://github.com/neurotechuoft/Biosignals-Board/tree/master/code/digital/doc/ads1299.pdf).

Currently the goal of this emulator is to imitate the ADS1299's:
  - Register Interface:
    - Reading and writing registers. Volatile bits and registers such as status register are not represented accurately.
  - System Command Interface
    - RDATAC
    - SDATAC
    - RDATA
    - RREG
    - WREG
  - Data Retrieval 
    - **NOTE**: This program will not simulate the analog functionality of the chip. Instead, dummy data (or test signal samples) will be sent to the master in place of the converted data that would come from the ADS1299.
    - Implement the ADS1299 Data retrieval protocol of pulsing DRDY and sending converted data over SPI to the master.
    
