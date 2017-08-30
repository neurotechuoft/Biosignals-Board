#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include <wiringPi.h>


#define	PIN_CS	 2 // Physical Pin 13 on the raspi bplus 
#define PIN_DRDY 3 // Physical Pin 15 on the raspi bplus

//SPI Command Definition Byte Assignments (Datasheet, pg. 35)
#define _WAKEUP 0x02 // Wake-up from standby mode
#define _STANDBY 0x04 // Enter Standby mode
#define _RESET 0x06 // Reset the device
#define _START 0x08 // Start and restart (synchronize) conversions
#define _STOP 0x0A // Stop conversion
#define _RDATAC 0x10 // Enable Read Data Continuous mode (default mode at power-up)
#define _SDATAC 0x11 // Stop Read Data Continuous mode
#define _RDATA 0x12 // Read data by command; supports multiple read back

#define ID 0x00
#define CONFIG1 0x01
#define CONFIG2 0x02
#define CONFIG3 0x03
#define LOFF 0x04
#define CH1SET 0x05
#define CH2SET 0x06
#define CH3SET 0x07
#define CH4SET 0x08
#define CH5SET 0x09
#define CH6SET 0x0A
#define CH7SET 0x0B
#define CH8SET 0x0C
#define BIAS_SENSP 0x0D
#define BIAS_SENSN 0x0E
#define LOFF_SENSP 0x0F
#define LOFF_SENSN 0x10
#define LOFF_FLIP 0x11
#define LOFF_STATP 0x12
#define LOFF_STATN 0x13
#define GPIO 0x14
#define MISC1 0x15
#define MISC2 0x16
#define CONFIG4 0x17


#define _RREG 0x20 // (also = 00100000) is the first opcode that the address must be added to for RREG communication
#define _WREG 0x40 // 01000000 in binary (Datasheet, pg. 35)



#define TCLK 0.4882815 // microseconds
#define FCLK 2.048
#define DEBUG 1 



// The SPI bus parameters
//	Variables as they need to be passed as pointers later on

const static char       *spiDev0  = "/dev/spidev0.0" ;
const static char       *spiDev1  = "/dev/spidev0.1" ;
const static uint8_t     spiBPW   = 8 ; // Bytes per word
const static uint16_t    spiDelay = 0 ;
static uint8_t	 mode = 1;


static uint32_t    spiSpeeds [2] ;
static int         spiFds [2] ;

#define __DEBUG__
	

unsigned char* dataTransfer (unsigned char _data)
{

  #ifdef __DEBUG__
	//printf("MOSI: %02x \n",_data);
  #endif

  unsigned char *data; 
  int len; 

  data = &_data;
  len = sizeof(_data);

  struct spi_ioc_transfer spi ;

  int channel = 0 ;

  memset (&spi, 0, sizeof (spi)) ;

  spi.tx_buf        = (unsigned long)data ;
  spi.rx_buf        = (unsigned long)data ;
  spi.len           = len ;
  spi.delay_usecs   = spiDelay ;
  spi.speed_hz      = spiSpeeds [channel] ;
  spi.bits_per_word = spiBPW ;

  ioctl (spiFds [channel], SPI_IOC_MESSAGE(1), &spi) ;

  #ifdef __DEBUG__
	printf("MISO:%02x \n",*data);
  #endif
  return data;
}


int spiSetup ()
{
  int fd ;

  int mode    = 1 ;	// Mode is 0, 1, 2 or 3
  int channel = 0 ;	// Channel is 0 or 1
  int speed = 3000000;
  if ((fd = open (channel == 0 ? spiDev0 : spiDev1, O_RDWR)) < 0)
	return -1;
  spiSpeeds [channel] = speed ;
  spiFds    [channel] = fd ;

// Set SPI parameters.

  if (ioctl (fd, SPI_IOC_WR_MODE, &mode)            < 0)
	return -1;  
  if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0)
	return -1;
  if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed)   < 0)
	return -1;
  return fd ;
}


unsigned char getDeviceId()
{
	unsigned char *data;
	dataTransfer(0x20);
	dataTransfer(0x00);
	data = dataTransfer(0x00);
	return *data;
}

unsigned char sendCommand(unsigned char data)
{
	return (*dataTransfer(data));
}

unsigned char readRegister(unsigned char data)
{
	unsigned char *_data;
	data = 0x20 + data;
	dataTransfer(data);
	dataTransfer(0x00);
	_data = dataTransfer(0x00);
	return *_data;
}

void writeRegister(unsigned char data, unsigned char data1)
{
	data = 0x40 + data;
	dataTransfer(data);
	dataTransfer(0x00);
	dataTransfer(data1);
	
}

void dataReady(void)
{
	delayMicroseconds(2); if(digitalRead(PIN_DRDY)) { printf("Got an interrupt on pin 15 \n"); }
}

int main()
{	
	int fd = spiSetup();
	
	if(fd < -1)
	{
		printf("bad file descriptor");
		
	}
	
	
	// Setup wiringPi
	wiringPiSetup ();
	
	// Setup interrupt 
	wiringPiISR (3, INT_EDGE_RISING, &dataReady) ;
	
	pinMode(PIN_CS, OUTPUT);
	pinMode(PIN_DRDY, INPUT);
	
	digitalWrite(PIN_CS, LOW);
	// 1000 milliseconds delay to make sure the chip select is all set
	delay(1000);
	
	// to receive the data
	unsigned char data;
	
	// Reset and send SDATAC to stop continuous mode
	sendCommand(_RESET);
	delayMicroseconds(20*TCLK);
	sendCommand(_SDATAC);
	delayMicroseconds(4*TCLK);	

	// Printing the current register configurations
	printf("device id: %02x \n", readRegister(0x00));
	printf("conf1g id: %02x \n", readRegister(0x01));
	printf("conf2g id: %02x \n", readRegister(0x02));
	printf("conf3g id: %02x \n", readRegister(0x03));
	printf("conf4g id: %02x \n", readRegister(0x17));


	// set reference
	writeRegister(CONFIG3,0xe0);

	// write certain registers
	writeRegister(CONFIG1, 0x96);
	writeRegister(CONFIG2, 0xc0);

	writeRegister(CH1SET, 0x04);
	writeRegister(CH2SET, 0x04);
	writeRegister(CH3SET, 0x04);
	writeRegister(CH4SET, 0x04);
	writeRegister(CH5SET, 0x04);
	writeRegister(CH6SET, 0x04);
	writeRegister(CH7SET, 0x04);
	writeRegister(CH8SET, 0x04);

	fflush(stdout);
	// send command START
	sendCommand(_START);

	
	// Start the on demand mode
	sendCommand(_RDATA);

	// Start the continuous mode 	
	//sendCommand(_RDATAC);

	// delay is in milliseconds
	delay(15);	
	

	// microsecond delay
	// delayMicroseconds(10);


	// now detect a toggle in the signal and do something
	
	int i;
	
	//unsigned double *alldata;

	for(i=0;i<26;i++)
	{
	   data = sendCommand(0x00);
	
	// make alldata from data
	}		

	sendCommand(_STOP);


	//while(1);
	
	// close spi?
	
	return 0;
	
	
}




