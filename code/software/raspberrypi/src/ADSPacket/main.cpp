#include "ADSPacket.h"
#include <math.h>
#include <time.h>       /* time */


#define PI 3.14159265
#define NUM_CHANNELS 8
#define NUM_PACKETS 500



// Get a random number between -2^23 and +2^23
// Use this function instead of getNextReading() for now
int getNextSample(int param, int chan) {
	int sample;
	int positive;

	// 
	srand(param * chan);
	positive = rand() & 1;

	srand(floor(((int) time(NULL) * chan) / (param + 1)));
	if (positive == 1)
		sample = rand() % ((int) (pow(2, 23)-1));
	else
		sample = -(rand() % ((int) (pow(2, 23)-1)));

	printf("Channel: %d, Sample: %d, Code: %x\n", chan, sample, sample);

	return sample;
}


// Return the next sample of a sine wave of frequency "freq"
// TODO: Move this function to a ADSEmulator class that is to be created
//		 vRef, gain, bitPerSample, will all be members of that class
// FIXME: Need to fix the conversion from sample (voltage) to output code
int getNextReading(int param, int chan, int freq) {
	double sample, LSB, vRef, nLevels;
	int  gain, bitsPerSample, quantized;

	vRef = 4.5;
	gain = 1;
	bitsPerSample = 24;

	// Raw signal has an amplitude of vRef and is *gain* times smaller than the value outputed from the ADC.
	sample = (2 * vRef * sin (param * freq * PI/180)) / gain;

	// The size of one code (LSB) of the 24 bit sample
	LSB = (2 * vRef) / (gain * (pow(2, bitsPerSample - 1) - 1));

	nLevels = sample / LSB;
	quantized = floor(sample / LSB);

	printf("\nChannel: %d, Freq: %d, Sample: %f, Levels: %f, quantized: %x\n", chan, freq, sample, nLevels, quantized);
	return quantized;
}

int main (int argc, char *argv[]) {

	int current_param = 0;
	int frequency_by_channel[NUM_CHANNELS];
	int seed;
	ADSPacket packet;

	//packet.verbose = true;

	// Make sure we generate different frequencies every time	
	//if (argc >= 2) {
	//	if (atoi(argv[1]) == 0) {
	//		seed = (int) (time(NULL));
	//	} else {
	//		seed = atoi(argv[1]);	
	//	}	
	//} else {		
	//	seed = (int) (time(NULL));
	//}


	// Make sure we generate different frequencies every time
	seed = (int) time(NULL);
	printf("Seed: %d\n\n", seed);
	srand(seed); 

	// Use a random integer between 0 and 100 as the frequency of the channel
	for (int i = 0; i < NUM_CHANNELS; i++) {
		frequency_by_channel[i] = rand() % 100;
	}

	while (current_param < NUM_PACKETS) {
		printf("---\n");
		printf("Packet #%d\n", current_param);
		printf("---\n");

		int data[NUM_CHANNELS];

		for (int j = 0; j < NUM_CHANNELS; j++) {
			// Get a random number 

			data[j] = getNextSample(current_param, j+1);

			// data[j] = getNextReading(current_param, j+1, frequency_by_channel[j]);
			
		}

		packet.createPacketFromReal(data);
		packet.printPacket();
		printf("\n---Waiting for next packet---\n\n");

		//for (int i = 0; i < 10000000; i++) {}
		
		current_param++;
	}
}
