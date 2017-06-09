
//
//  ADS1299Manager.h
//  Part of the Arduino Library for the ADS1299 Shield
//  Created by Chip Audette, Fall 2013
//


#ifndef ____ADS1299Manager__
#define ____ADS1299Manager__

#include <ADS1299.h>

//Pick which version of OpenBCI you have
#define OPENBCI_V1 (1)    //Sept 2013
#define OPENBCI_V2 (2)    //Oct 24, 2013
#define OPENBCI_NCHAN_PER_BOARD (8)  // number of EEG channels

/*   Arduino Uno - Pin Assignments
  SCK = 13
  MISO [DOUT] = 12
  MOSI [DIN] = 11
  CS = 10; 
  RESET = 9;
  DRDY = 8;
*/
#define PIN_DRDY (8)
#define PIN_RST (9)
#define PIN_CS (10)
#define SCK_MHZ (4)

//gainCode choices
#define ADS_GAIN01 (0b00000000)
#define ADS_GAIN02 (0b00010000)
#define ADS_GAIN04 (0b00100000)
#define ADS_GAIN06 (0b00110000)
#define ADS_GAIN08 (0b01000000)
#define ADS_GAIN12 (0b01010000)
#define ADS_GAIN24 (0b01100000)

//inputCode choices
#define ADSINPUT_NORMAL (0b00000000)
#define ADSINPUT_SHORTED (0b00000001)
#define ADSINPUT_TESTSIG (0b00000101)

//test signal choices...ADS1299 datasheet page 41
#define ADSTESTSIG_AMP_1X (0b00000000)
#define ADSTESTSIG_AMP_2X (0b00000100)
#define ADSTESTSIG_PULSE_SLOW (0b00000000)
#define ADSTESTSIG_PULSE_FAST (0b00000001)
#define ADSTESTSIG_DCSIG (0b00000011)
#define ADSTESTSIG_NOCHANGE (0b11111111)

//Lead-off signal choices
#define LOFF_MAG_6NA (0b00000000)
#define LOFF_MAG_24NA (0b00000100)
#define LOFF_MAG_6UA (0b00001000)
#define LOFF_MAG_24UA (0b00001100)
#define LOFF_FREQ_DC (0b00000000)
#define LOFF_FREQ_7p8HZ (0b00000001)
#define LOFF_FREQ_31p2HZ (0b00000010)
#define LOFF_FREQ_FS_4 (0b00000011)
#define PCHAN (1)
#define NCHAN (2)
#define BOTHCHAN (3)

#define OFF (0)
#define ON (1)

//binary communication codes for each packet
#define PCKT_START 0xA0
#define PCKT_END 0xC0

class ADS1299Manager : public ADS1299 {
  public:
    void initialize(void);                                     //initialize the ADS1299 controller.  Call once.  Assumes OpenBCI_V2
    void initialize(int version,boolean isDaisy);              //initialize the ADS1299 controller.  Call once.  Set which version of OpenBCI you're using.
    void setVersionOpenBCI(int version);			//Set which version of OpenBCI you're using.
    void reset(void);                                          //reset all the ADS1299's settings.  Call however you'd like
    boolean isChannelActive(int N_oneRef);
    void activateChannel(int N_oneRef, byte gainCode,byte inputCode); //setup the channel 1-8
    void deactivateChannel(int N_oneRef);                            //disable given channel 1-8
    void configureLeadOffDetection(byte amplitudeCode, byte freqCode);  //configure the lead-off detection signal parameters
    void changeChannelLeadOffDetection(int N_oneRef, int code_OFF_ON, int code_P_N_Both);
    void configureInternalTestSignal(byte amplitudeCode, byte freqCode);  //configure the test signal parameters
    void start(void);
    void stop(void);
    int isDataAvailable(void);
    void printChannelDataAsText(int N, long int sampleNumber);
    void writeChannelDataAsBinary(int N, long int sampleNumber);
    void writeChannelDataAsBinary(int N, long int sampleNumber, boolean useSyntheticData);
    void writeChannelDataAsBinary(int N, long int sampleNumber, long int auxValue);
    void writeChannelDataAsBinary(int N, long int sampleNumber, long int auxValue, boolean useSyntheticData);
    void writeChannelDataAsBinary(int N, long int sampleNumber, boolean sendAuxValue,long int auxValue, boolean useSyntheticData);
    void writeChannelDataAsOpenEEG_P2(long int sampleNumber);
    void writeChannelDataAsOpenEEG_P2(long int sampleNumber, boolean useSyntheticData);
    void printAllRegisters(void);
    void setSRB1(boolean desired_state);
    void alterBiasBasedOnChannelState(int N_oneRef);
    void deactivateBiasForChannel(int N_oneRef);
    void activateBiasForChannel(int N_oneRef);
    void setAutoBiasGeneration(boolean state);
    
    
  private:
    boolean use_neg_inputs;
    boolean use_SRB2[OPENBCI_NCHAN_PER_BOARD];
    boolean use_channels_for_bias;
    boolean use_SRB1(void);
    long int makeSyntheticSample(long sampleNumber,int chan);
    int n_chan_all_boards;
};

#endif