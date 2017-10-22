#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "stdlib.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "string.h"

#define BUFSIZE 1024


#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include <wiringPi.h>


#define	PIN_CS	 2 // Physical Pin 13 on the raspi bplus 
#define PIN_DRDY 3 // Physical Pin 15 on the raspi bplus
#define PIN_START 6 //  Physical Pin 22 on the pi

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

#define UDP_ON

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

//#define __DEBUG__
	
int sample_no = 0;

unsigned char* dataTransfer (unsigned char _data)
{

  #ifdef __DEBUG__
	printf("MOSI: %02x \n",_data);
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
	delayMicroseconds(2);
	/*	
	if(digitalRead(PIN_DRDY)) 
	{ 	
		int i;

        //unsigned double *alldata;
        	uint32_t status = 0;

        	for (i = 0;i<2;++i) {
                	status |= sendCommand(0x00);
                	status = status << 8;
        	}
        	status |= sendCommand(0x00);

        	printf("\n Sample: %d \nStatus Register: 0x%lx\n",sample_no+1,status);

        	uint32_t channels[8] = {0,0,0,0,0,0,0,0};

        	for (i = 0; i<8;++i)
        	{
                	int j;
                	for (j = 0;j<2;++j) {
                        	channels[i] |= sendCommand(0x00);
                        	channels[i] = channels[i] << 8;
                	}
                	channels[i] |= sendCommand(0x00);
                	//printf("Channel %d: 0x%lx\n",i+1,channels[i]);
        	}
		sample_no ++;

	}
	*/
}

int main(int argc, char *argv[])
{
#ifdef UDP_ON	
	if(argc != 3){
      	   fprintf(stderr,"Usage: %s <server_ip> <server_port>\n", argv[0]);
                exit(0);
        }
	
	//setup TCP client to connect to PC

	struct sockaddr_in servaddr;
        int sockfd;
        char buf[BUFSIZE];
        char mesg[BUFSIZE];
        uint16_t port;

        if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0 )) < 0 ){
                perror("invalid socket");
                exit(0);
        }
        sscanf(argv[2],"%d",&port);

        bzero( &servaddr, sizeof(servaddr ));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);

  	/* convert address, e.g., 127.0.0.1, to the right format */
        if( inet_pton( AF_INET, argv[1], &servaddr.sin_addr ) <= 0 ){
                perror( "inet_pton for address" );
                exit(0);
        }

        if( connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ){
                perror( "connect to associative memory at server" );
                exit(0);
        }
#endif

	//SPI setup with analog front end
	int fd = spiSetup();
	
	if(fd < -1)
	{
		printf("bad file descriptor");
		
	}
	
	
	// Setup wiringPi
	wiringPiSetup ();
	
	// Setup interrupt 
	wiringPiISR (3, INT_EDGE_FALLING, &dataReady) ;
	
	pinMode(PIN_CS, OUTPUT);
	pinMode(PIN_DRDY, INPUT);
 	pinMode(PIN_START, OUTPUT);

	//digitalWrite(PIN_START, LOW);
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
	
	writeRegister(MISC1, 0x20);
	// write certain registers
	writeRegister(CONFIG1, 0x96);
	writeRegister(CONFIG2, 0xc0);

	writeRegister(CH1SET, 0x50);
	writeRegister(CH2SET, 0x50);
	writeRegister(CH3SET, 0x50);
	writeRegister(CH4SET, 0x50);
	writeRegister(CH5SET, 0x50);
	writeRegister(CH6SET, 0x50);
	writeRegister(CH7SET, 0x50);
	writeRegister(CH8SET, 0x50);

	fflush(stdout);
	// send command START
	sendCommand(_START);
	size_t k;
	
	struct timespec tstart = {0,0}, tend = {0,0};

	for (k =0;;++k) {
	
	delay(2);
	// Start the on demand mode
	sendCommand(_RDATA);

	// Start the continuous mode 	
	//sendCommand(_RDATAC);

	// delay is in milliseconds
	delay(5);

	digitalWrite(PIN_START, HIGH);
        //sendCommand(_START);
	clock_gettime(CLOCK_MONOTONIC, &tstart);

	// microsecond delay
	delayMicroseconds(10);


	// now detect a toggle in the signal and do something
	
	int i;
	
	//unsigned double *alldata;
	uint32_t status = 0;
	
	for (i = 0;i<2;++i) {
		status |= sendCommand(0x00);
		status = status << 8;
	}
	status |= sendCommand(0x00);	

	printf("\n Sample: %d \nStatus Register: 0x%lx\n",k+1,status);
	
	uint32_t channels[8] = {0,0,0,0,0,0,0,0};
	
	for (i = 0; i<8;++i)
	{
		int j;
		for (j = 0;j<2;++j) {
                	channels[i] |= sendCommand(0x00);
                	channels[i] = channels[i] << 8;
        	}
        	channels[i] |= sendCommand(0x00);
		printf("Channel %d: 0x%lx\n",i+1,channels[i]);
	}
	
#ifdef UDP_ON	
	send(sockfd,channels, sizeof (channels), 0);
#endif	
	}

	//while(sample_no < 128);
	sendCommand(_SDATAC);
	delayMicroseconds(4*TCLK);


	clock_gettime(CLOCK_MONOTONIC, &tend);
	
	delay(5);

	sendCommand(_STOP);
        
        printf("Seconds taken for 128 samples = %.6f\n",((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
	
	// close spi?
	
#ifdef TCP_ON
	close(sockfd);
#endif
	return 0;
	
	
}



