# ADSPacket

This class consists of two files: ADSPacket.h and ADSPacket.cpp. It is used to encapsulate the contents of an ADS1299 data packet that is outputted from the ADS in RDATAC (Read Data Continuous Mode). Note that the contents are stored in such a way that they can be sent over SPI byte by byte MSB first.

 - The packet structure is:
   - Status       (24 bits)
   - Channel 1    (24 bits)
   - Channel 2    (24 bits)
   - Channel 3    (24 bits)
   - Channel 4    (24 bits)
   - Channel 5    (24 bits)
   - Channel 6    (24 bits)
   - Channel 7    (24 bits)
   - Channel 8    (24 bits)

Right now, the class can take in an array of 8 floating point numbers (one number for each channel) and create a packet which is stored in the **current_packet[]** member with the **status** member prepended in front.

### Files
 - main.cpp is the program used to test the class. It generates random 24-bit numbers in the acceptable range for the ADS1299 and creates 500 ADSPacket objects from them. These objects are printed.
 - A Makefile is included to compile the required files into an executable **run.exe**.
 - Run './print_packets' to print the packets to terminal
 - Run './log_packets' to save the output to ./packets.log file.
 -

### To-Do:
 - Add functions to send **current_packet[]** over SPI
 - The purpose of this code is to eventually upload it to an Arduino and send packets to the Raspberry Pi over SPI.
