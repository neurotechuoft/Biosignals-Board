// ADS1299 Constants
#ifndef _Definitions_h
#define _Definitions_h

// Board specific settings
// fclk = 2.048 Mhz, TCLK = 488.2815 nanoseconds	
#define TCLK 		0.4882815 	// microseconds
#define FCLK 		2.048
#define ADC_RES		24
#define __DEBUG__


// Digital Pins
#define PIN_DRDY  RPI_BPLUS_GPIO_J8_15 	// Make Pin 15 the DRDY pin
#define PIN_CS    RPI_BPLUS_GPIO_J8_24  // Make Pin 24 (CE0) the CS pin
#define PIN_RESET RPI_BPLUS_GPIO_J8_18  // Make Pin 18 the RESET pin

// Settings spefific to our project
#define SPS            250  // 250 samples per sample
#define VREF	       4.5  // 
#define DVDD	       3.0  // Digital VDD
#define AVDD	       5.0  // Analog VDD

////////////////////////////////// SPI ////////////////////////////////////////////////

//SPI Command Definition Byte Assignments (Datasheet, pg. 35)
#define _WAKEUP        0x02 // Wake-up from standby mode
#define _STANDBY       0x04 // Enter Standby mode
#define _RESET         0x06 // Reset the device
#define _START         0x08 // Start and restart (synchronize) conversions
#define _STOP          0x0A // Stop conversion
#define _RDATAC        0x10 // Enable Read Data Continuous mode (default mode at power-up)
#define _SDATAC        0x11 // Stop Read Data Continuous mode
#define _RDATA         0x12 // Read data by command; supports multiple read back

#define _RREG          0x20 // (also = 00100000) is the first opcode that the address must be added to for RREG communication
#define _WREG          0x40 // 01000000 in binary (Datasheet, pg. 35)

//////////////////////////// Data Retrieval Modes ////////////////////////////////////

#define RDATAC_MODE    0x10
#define SDATAC_MODE    0x11
#define RDATA_MODE	   0x12

//////////////////////////////////////////////////////////////////////////////////////

//////////////// ADS1299 Control Register Base Addresses /////////////////////////////

// ID:     ID Control Register
// The read-only ID control register is programmed during device manufacture to indicate device characteristics (pg. 45)
// Reset: 0xXE
// Fields:
//	[7:5]	REV_ID 
//	[4:4] 	Reserved
//	[3:2] 	DEVICE_ID
//	[1:0] 	NU_CH
#define ID             0x00 

// CONFIG1: Configuration Register 1
// This register configures the !DAISY_EN bit, clock, and data rate. (pg. 46)
// Reset: 0x96
// Fields:
//	[7:7]	Reserved
//	[6:6]  !DAISY_EN
//	[5:5]	CLK_EN
//	[4:3]	Reserved
//	[2:0]	DR
#define CONFIG1        0x01

// CONFIG2: Configuration Register 2
// This register configures the test signal generation (pg. 47)
// Reset: 0xC0
// Fields:
//	[7:5]	Reserved
//	[4:4]	INT_CAL
//	[3:3]	Reserved
//	[2:2]	CAL_AMP
//	[1:0]	CAL_FREQ
#define CONFIG2        0x02

// CONFIG3: Configuration Register 3
// Configuration register 3 configures either an internal or exteral reference and BIAS operation. (pg. 48)
// Reset: 0x60
// Fields:
//	[7:7]	!PD_REFBUF
//	[6:5]  	Reserved
//	[4:4]	BIAS_MEAS
//	[3:3]	BIASREF_INT
//	[2:2]	!PD_BIAS
//	[1:1]	BIAS_LOFF_SENS
//	[0:0]	BIAS_STAT
#define CONFIG3        0x03

// LOFF: Lead-Off Control Register
// The lead-off control register configures the lead-off detection operation. (pg. 49)
// Reset: 0x00
// Fields:
//	[7:5]	COMP_TH
//	[4:4]  	Reserved
//	[3:2]	ILEAD_OFF
//	[1:0]	FLEAD_OFF
#define LOFF           0x04

// CHnSET: Individual Channel Settings (n = 1 to 8)
// The CH[1:8]SET control register configures the power mode, PGA gain, and multiplexer settings channels. See
// the Input Multiplexer section for details. CH[2:8]SET are similar to CH1SET, corresponding to the respective
// channels. (pg. 50)
// Reset: 0x61
// Fields:
//	[7:7]	PDn
//	[6:4]  	GAINn
//	[3:3]	SRB2
//	[2:0]	MUXn
#define CH1SET         0x05
#define CH2SET         0x06
#define CH3SET         0x07
#define CH4SET         0x08
#define CH5SET         0x09
#define CH6SET         0x0A
#define CH7SET         0x0B
#define CH8SET         0x0C

// BIAS_SENSP: Bias Drive Positive Derivation Register
// This register controls the selection of the positive signals from each channel for bias voltage (BIAS) derivation.
// See the Bias Drive (DC Bias Circuit) section for details. (pg. 51)
// Reset: 0x00
// Fields:
//	[n]		BIASP(n+1)
#define BIAS_SENSP     0x0D

// BIAS_SENSP: Bias Drive Positive Derivation Register
// This register controls the selection of the negative signals from each channel for bias voltage (BIAS) derivation.
// See the Bias Drive (DC Bias Circuit) section for details. (pg. 52)
// Reset: 0x00
// Fields:
//	[n]		BIASN(n+1)
#define BIAS_SENSN     0x0E

// LOFF_SENSP: Positive Signal Lead-Off Detection Register
// This register selects the positive side from each channel for lead-off detection. See the Lead-Off Detection
// section for details. The LOFF_STATP register bits are only valid if the corresponding LOFF_SENSP bits are set
// to 1. (pg. 53)
// Reset: 0x00
// Fields:
//	[n]		LOFFP(n+1)
#define LOFF_SENSP     0x0F

// LOFF_SENSN: Negative Signal Lead-Off Detection Register
// This register selects the negative side from each channel for lead-off detection. See the Lead-Off Detection
// section for details. The LOFF_STATN register bits are only valid if the corresponding LOFF_SENSN bits are set
// to 1. (pg. 54)
// Reset: 0x00
// Fields:
//	[n]		LOFFP(n+1)
#define LOFF_SENSN     0x10


// LOFF_FLIP: Lead-Off Flip Register
// This register controls the direction of the current used for lead-off derivation. See the Lead-Off Detection section
// for details. (pg. 55)
// Reset: 0x00
// Fields: 
//	[n]		LOFF_FLIP(n+1)
#define LOFF_FLIP      0x11

// LOFF_STATP: Lead-Off Positive Signal Status Register
// This register stores the status of whether the positive electrode on each channel is on or off. See the Lead-Off
// Detection section for details. Ignore the LOFF_STATP values if the corresponding LOFF_SENSP bits are not set
// to 1.
// Reset: 0x00
// Fields:
//	[n] IN(n+1)P_OFF
#define LOFF_STATP     0x12

// LOFF_STATN: Lead-Off Negative Signal Status Register
// This register stores the status of whether the negative electrode on each channel is on or off. See the Lead-Off
// Detection section for details. Ignore the LOFF_STATN values if the corresponding LOFF_SENSN bits are not set
// to 1.
// Reset: 0x00
// Fields:
//	[n] IN(n+1)N_OFF
#define LOFF_STATN     0x13

// GPIO: General-Purpose I/O Register
// The general-purpose I/O register controls the action of the three GPIO pins. When RESP_CTRL[1:0] is in mode
// 01 and 11, the GPIO2, GPIO3, and GPIO4 pins are not available for use.
// Reset: 0x0F
// Fields:
//	[7:4]	GPIOD
//	[3:0] 	GPIOC
#define GPIO           0x14

// MISC1: Miscellaneous 1 Register
// This register provides the control to route the SRB1 pin to all inverting inputs of the four, six, or eight channels
// (ADS1299-4, ADS1299-6, or ADS1299).
// Reset: 0x00
// Fields:
//	[7:6]	Reserved
//	[5:5] 	SRB1
//	[4:0] 	Reserved
#define MISC1          0x15

// MISC2: General-Purpose I/O Register
// This register is reserved for future use.
// Reset: 0x00
// Fields:
//	[7:0]	Reserved
#define MISC2          0x16

// CONFIG4: Configuration Register 4
// This register configures the conversion mode and enables the lead-off comparators.
// Reset: 0x00
// Fields:
//	[7:4]	Reserved
//	[3:3]	SINGLE_SHOT
//	[2:2]	Reserved
//	[1:1]	!SINGLE_SHOT
//	[0:0]	Reserved
#define CONFIG4        0x17

//////////////// ADS1299 Control Register Default Values /////////////////////////////

// Note for unknown bits, set default to 1 so when ANDing in check, does not corrupt output.

// ID is unknown
#define ID_DEFAULT 			0xFE

#define CONFIG1_DEFAULT		0x96	
#define CONFIG2_DEFAULT		0xA0
#define CONFIG3_DEFAULT		0x60

#endif
