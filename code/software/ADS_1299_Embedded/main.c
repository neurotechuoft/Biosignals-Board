/*Include the needed libraries for functionality*/
#include <stdio.h>


int main(int argc, char **argv)
{
    // If error in intializing bcm
    if (!bcm2835_init())
    {printf("Failed to init the Raspberry PI\n");
	return 1;}
	// Initialize BCM Configuration to communicate with chip
	ADS1299_init();

    // bootup sequence
    ADS1299_bootup();
 
    return 0;
}
