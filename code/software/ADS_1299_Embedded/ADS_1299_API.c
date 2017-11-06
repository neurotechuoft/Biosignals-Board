#define RESET_PIN 10
#include <stdio.h>
#include <wiringPi.h>


void ADS1299_bootup(){
	/* Function: Boots up ADS1299
	   Returns: None */
    wiringPiSetup();
    delay(50);
    
    pinMode(RESET_PIN,OUTPUT); // set reset pin to output
    
    digitalWrite(RESET_PIN,LOW); // write the pin to low
    delayMicroseconds(4);
    
    digitalWrite(RESET_PIN,HIGH); // write pin to high
    delayMicroseconds(20); // delay time for bootup
    
    return 0;

}

int main(int argc, char **argv){

    if (wiringPiSetup () == -1)
        return 1 ;

    ADS1299_bootup();
    
    return 0;


}

