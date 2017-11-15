/*Include the needed libraries for functionality*/
#include "ADS1299_definitions.h"
#include "ADS1299_bcm2835.h"
#include "ads1299.h"

int main(int argc, char **argv)
{
    // If error in intializing bcm
    if (!bcm2835_init()) {
	printf("Failed to init the Raspberry PI");
	return 1;
    }

    // Initialize BCM Configuration to communicate with chip
    ADS1299_init();

    // bootup sequence
    ADS1299_bootup();

    // Test registers' default values
    ADS1299_test_registers(); 
 
    return 0;
}
