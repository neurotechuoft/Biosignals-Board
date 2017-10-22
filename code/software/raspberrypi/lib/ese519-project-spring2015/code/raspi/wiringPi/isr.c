#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

static volatile int globalCounter [8] ;

#define PIN_DRDY 3

/*
 * myInterrupt:
 *********************************************************************************
 */

void myInterrupt3 (void) { delayMicroseconds(5); if(digitalRead(PIN_DRDY)) { ++globalCounter [3] ; printf("Got an interrupt on pin 3"); fflush(stdout);}}


/*
 *********************************************************************************
 * main
 *********************************************************************************
 */

int main (void)
{
  int gotOne, pin ;
  int myCounter [8] ;

  for (pin = 0 ; pin < 8 ; ++pin) 
    globalCounter [pin] = myCounter [pin] = 0 ;

  wiringPiSetup () ;

  wiringPiISR (3, INT_EDGE_RISING, &myInterrupt3) ;


  for (;;)
  {
    gotOne = 0 ;
    printf ("Waiting ... ") ; fflush (stdout) ;

    for (;;)
    {
      for (pin = 0 ; pin < 8 ; ++pin)
      {
	if (globalCounter [pin] != myCounter [pin])
	{
	  printf (" Int on pin %d: Counter: %5d\n", pin, globalCounter [pin]) ;
	  myCounter [pin] = globalCounter [pin] ;
	  ++gotOne ;
	}
      }
      if (gotOne != 0)
	break ;
    }
  }

  return 0 ;
}



