
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class ADSPacket {

	public:

		// Constructor
		ADSPacket();

		bool populated;

		// Status
		// (1100 + LOFF_STATP + LOFF_STATN + bits[4:7] of the GPIO register)
		int status;

		// Real channel data as a voltage reading as an integer
		int real_channel_data[8];

		// An ADS1299 data packet has the following format:
		// 24 status bits + (8 channels * 24 bits/channel) = 216 bits or 27 bytes
		int current_packet[9];

		bool verbose;

		// Functions
		void createPacketFromReal(int * real_data);
		void printPacket();

		// Debugging function

};
