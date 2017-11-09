#include "bcm2835.h"
#include <ADS1299_bcm.h>
#define RESET RPI_BPLUS_GPIO_J8_18
 
void ADS1299_bootup(){
/* Function: Bootup Routine of the ADS1299
   Return: None */
	
	// Set the pin to output
	bcm2835_gpio_fsel(RESET, BCM2835_GPIO_FSEL_OUTP);

	//Set the pin to low and delay (TIME TO BE DECIDED BASED ON TESTING)
	bcm2835_gpio_write(RESET, LOW);
	delay(10);

	//set pin to high to reset and bootup; delay (TIME TO BE DECIDED DURING TESTING)
	bcm2835_gpio_write(RESET,HIGH);
	delay(100);

}




int main(int argc, char **argv)
{
    // If error in intializing bcm
    if (!bcm2835_init())
	return 1;
 
    // bootup sequence
    ADS1299_bootup();
 
    return 0;
}
