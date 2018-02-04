/*Include the needed libraries for functionality*/
#include "ADS1299_definitions.h"
#include "ADS1299_bcm2835.h"
#include "ads1299.h"
#include <math.h>

int main(int argc, char **argv)
{
    // If error in intializing bcm
    if (!bcm2835_init()) {
	  printf("Failed to init the Raspberry PI");
	   return 1;
    }

    bool bootup_success = false;
    int num_attempts = 1;
    int attempt = 1;

    //ADS1299_init();

    // Perform boot-up and test CONFIG registers for default values.
    while (attempt <= num_attempts && !bootup_success) {

        // Initialize BCM Configuration to communicate with chip
        ADS1299_init();
	
	delayMicroseconds(100);
        // bootup sequence
        ADS1299_bootup();
	
	   // Send SDATAC before reading/writing registers
	   transferCmd(_SDATAC);

	// Write necessary registers/pins for our ADS1299 configuration	
	ADS1299_config();
	
        printf("\nAttempt #%d\n", attempt);

        // Test registers' default values
        bootup_success = ADS1299_test_registers();

        if (bootup_success) {
            printf("\n-- ADS1299 Boot-up Successful --\n");
        } else {
            printf("\n-- ADS1299 Boot-up Failed --\n");
        }

        attempt++;
        // Delay 500 us before moving on or reattempting bootup
        bcm2835_delayMicroseconds(500);
	
    } 

    if (!bootup_success) {
        printf("\nAttempts %d of %d to boot ADS1299 unsuccessful. Aborting...\n", num_attempts, num_attempts);
	   return 1;
    }

    return 0;
}
