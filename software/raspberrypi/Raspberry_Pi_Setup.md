## Raspberry Pi Set-up

### Hardware Configuration
 - Model: Raspberry Pi 3 Model B
 - OS: Raspbian Jessie (2017) (Image size: 4 GB)
 - SD Card: Class 4 (4 MB/s Read/Write speed) MicroSD Card, 8 GB storage

### Finished 
 - Set-up ssh
 - Set-up basic environment settings
 - Clone Biosignals-Board Git Repository
 - Setup VNC
 - Install and setup wiringPi. Follow instructions here: http://wiringpi.com/download-and-install/
 - Test wiringPi SPI Library: http://wiringpi.com/reference/spi-library/
   - Verified wiringPi blink.c example can blink LED on Arduino
 - Verified SPI send/receive functionality by running loopback test: ./RaspberryPi/spi_tests/spi_loopback

### To-do
 - Setup Wi-Fi connection on UofT Network

### Notes
 - Biosignals-Board Github repo can be jumped to by running 'cd $BIOSIGNALS_BOARD'
 
### Issues
 - Cannot ping external ip-addresses other than localhost
