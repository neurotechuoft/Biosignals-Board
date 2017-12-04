#include <SPI.h>
#include <string.h>

byte recv_buff[3];
int pos = 0;
byte process_it;

byte regs_defaults[5] = {0x1E, 0x96, 0xA0, 0x60, 0xFF};

enum {WAKEUP, STANDBY, RESET, START, STOP, RDATAC, SDATAC, RDATA, RREG, WREG, _NULL, NOT_WREG_RREG};

int current_state;
int pin_reset = 8;

// Instruction decoding
byte opcode;
byte byte_decoded;
int address;
String sys_command;
bool req_reg_access = false;

byte prev_byte;

void setup() {
  Serial.begin(115200);

  SPCR |= bit(SPE);
  pinMode(MISO, OUTPUT);
  pinMode(pin_reset, INPUT);

  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE1));
  
  process_it = false;
  SPI.attachInterrupt();
  SPDR = 0;

}


void loop() {
  
  if (process_it) {
      //Serial.println("Received command");
      process_it = false;

//    Serial.print("Raw: 0x");
//    Serial.print(recv_buff[0], HEX);
//    Serial.print(" 0x");
//    Serial.println(recv_buff[1], HEX);
//    Serial.print("Read or write: ");
//    Serial.println(req_reg_access);
//    Serial.print("Byte Decoded: 0x");
//    Serial.println(byte_decoded, HEX);
    
//    Serial.print("Command: ");
//    Serial.print(sys_command);
//    Serial.print(" Address: ");
//    Serial.println(address, HEX);
//    Serial.println("");

  }
}

// Depending on the command recieved from the Pi, send a known register value back to the Pi.
ISR(SPI_STC_vect) {
  byte rec_byte = SPDR;
  process_it = false;
  if (rec_byte == 0x20) {
    prev_byte = 0x20;
  } else if (rec_byte == 0x21) {
    prev_byte = 0x21;
  } else if (rec_byte == 0x22) {
    prev_byte = 0x22;
  } else if (rec_byte == 0x23) {
    prev_byte = 0x23;
  } else if (rec_byte == 0x00) {
     process_it = true;
     if (prev_byte == 0x20) {
      SPDR = regs_defaults[0];
    } else if (prev_byte == 0x21) {
      SPDR = regs_defaults[1];
    } else if (prev_byte == 0x22) {
      SPDR = regs_defaults[2];
    } else if (prev_byte == 0x23) {
      SPDR = regs_defaults[3];  
    } else {
      process_it = false;
    }
    prev_byte = 0x00;
  } else {
    process_it = false;
  }
    
//  if (pos == 0) 
//    decode_opcode(rec_byte);
//
//  if (pos < sizeof recv_buff) {
//    recv_buff[pos++] = rec_byte;
//    SPDR = 0;
//    process_it = false;
//
//    if (req_reg_access) {
//      if (pos == 2) {
//        address = rec_byte;
//        SPDR = regs_defaults[address];
//      } else if (pos == 3) { // Done accepting two-byte command
//        pos = 0;
//        process_it = true;
//      } 
//    } else { // Not WREG or RREG
//      pos = 0;
//      process_it = true;
//    }
//  } else {
//    pos = 0;
//    SPDR = 0;
//  }
}

// Function currently takes too long to be run in ISR.
void decode_opcode(byte _recv_opcode) {
  // Extract opcode
  byte first_byte = _recv_opcode;
  byte_decoded = first_byte;

  // 3 MSB's are 0 so the command is not WREG or RREG
  if (first_byte >> 5 == 0x00) {
    req_reg_access = false;
    address = 0x05;
  } else {                         
    req_reg_access = true;
    address = _recv_opcode & 0x1F;
    if (first_byte >> 5 == 0x1) {
      opcode = RREG;
      sys_command = "RREG";
    } else if (first_byte >> 5 == 0x2) {
      opcode = WREG;
      sys_command = "WREG";
    } else {
      req_reg_access = false;
      opcode = NOT_WREG_RREG;
      sys_command = "NOT_WREG_RREG";
    }
  }

  if (!req_reg_access) {

      if (first_byte == 0x2) {
        opcode      = WAKEUP; 
        sys_command = "WAKEUP";
      } else if (first_byte == 0x4) {
        opcode      = STANDBY;
        sys_command = "STANDBY";
      }else if (first_byte == 0x6) {
        opcode      = RESET;
        sys_command = "RESET";
      } else if (first_byte == 0x8) {
        opcode      = START;
        sys_command = "START";
      } else if (first_byte == 0xA) {
        opcode      = STOP;
        sys_command = "STOP";
      } else if (first_byte == 0x10) {
        opcode      = RDATAC;
        sys_command = "RDATAC";
      } else if (first_byte == 0x11) {
        opcode      = SDATAC;
        sys_command = "SDATAC";
      } else if (first_byte == 0x12) {
        opcode      = RDATA;
        sys_command = "RDATA";
      } else {
        opcode      = _NULL;
        sys_command = "NULL";
      }
  }
}



