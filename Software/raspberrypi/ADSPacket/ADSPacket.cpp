#include "ADSPacket.h"

ADSPacket::ADSPacket() {

	populated = false;
	verbose = false;

	status = 0b00001010;


	for (int i = 0; i < 8; i++) {
		real_channel_data[i] = 0;
	}
}

// Populate the current_packet member with a status and channel_data
void ADSPacket::createPacketFromReal(int * real_data) {

	int inByte;
	int channel_data;

	for (int i = 0; i < 8; i++) {
		real_channel_data[i] = real_data[i];
	}

	// Clear array
	for (int p = 0; p < 9; p++) {
		current_packet[p] = 0;
	}

	current_packet[0] = status;

	// Create packet by reading in data in 8 channels, each channel being 3 bytes.
	for (int i = 0; i < 8; i++) {
		//current_packet[i+1] = real_channel_data[i];
		channel_data = *(real_channel_data + i);
		// Need to convert from little-endian to big-endian so data can be sent MSB first
		// This is done by reversing the order of bytes. So:
		// a1c2e3 becomes e3c2a1.
		for (int j = 2; j >= 0; j--) {
			// For now, grab data from channel_data array
			inByte = channel_data & 0xFF; // Get lower 8 bits of channel data
			
			if (verbose) printf("Creating packet: Channel %d, Byte %d, Data: %x\n", i + 1, j, inByte);

			// Shift new byte up enough so it does not clobber existing data
			// Bitwise OR with existing data in current_packet
			inByte = inByte << 8*j;
			current_packet[i+1] = current_packet[i+1] | inByte;

			// Shift down next 8 bits of channel data
			channel_data = channel_data >> 8;
		}
		if (verbose) printf("Done channel: Channel %x\n\n", current_packet[i+1]);
	}
	populated = true;
}

void ADSPacket::printPacket() {
	if (!populated) {
		printf("Warning: packet is empty. Call ADSPacket::createPacketFromReal() to populate the packet.\n");
	}

	if (verbose) {
		for (int i = 0; i < 9; i++) {
			if (i==0) printf("\nStatus: %x\n", current_packet[0]);
			else {
				printf("Channel %d: %x\n", i, current_packet[i]);
			}
		}
	}

	printf("\nPacket: -");
	for (int j = 0; j < 9; j++) {
		printf("%x--", current_packet[j]);
	}
	printf("\n");
}
