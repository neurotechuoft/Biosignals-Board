## Hardware Tests

This folder will contain short programs to test various hardware on the Raspberry Pi

### SPI Loopback
 - Location: ./spi_loopback
 - Standard loopback test to verify send and received functionality of Raspberry Pi
 - Followed instructions here: https://www.raspberrypi.org/documentation/hardware/raspberrypi/spi/README.md
 - To test:
   - Connect SPI_MISO (Pin 21) and SPI_MOSI (Pin 19). No SCLK and CS is needed in loopback test
   - Run bash ./loopback script
   - Compare output printed to tx[] array in spidev_test.c
