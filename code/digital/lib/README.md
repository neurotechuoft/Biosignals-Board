# Libraries
* This directory includes the following libraries:
  - Modified ADS1299 Library
    - Original Libary: https://github.com/OpenBCI/OpenBCI-V2hardware-DEPRECATED/tree/master/Arduino/Libraries/ADS1299
    - Modifications:
      - Added PWDN pin to ADS1299::initialize
      - Incorporate use of Arduino SPI library into communication functions
