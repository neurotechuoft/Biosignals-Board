#include <SPI.h>
#include <string.h>

byte process_it;

// Register Read/Write settings
bool read_reg, write_reg, rw_done, proc_wreg;
byte base_addr;
int rw_offset;
int n_regs;
byte opcode;

byte rec_byte;
byte regs_defaults[5] = {0x1E, 0x96, 0xA0, 0x60, 0xFF};

enum ads1299_state {WAKEUP, STANDBY, RESET, START, STOP, RDATAC, SDATAC, RDATA, RREG, WREG, NULL_STATE, NOT_WREG_RREG};

String current_state;

// Pins
int pin_pwdn = 7;
int pin_reset = 8;

int pin_cs = 10;
int pin_mosi = 11;
int pin_miso = 12;
int pin_sclk = 13;


void setup() {
  Serial.begin(115200);

  SPCR |= bit(SPE);
  pinMode(MISO, OUTPUT);
  pinMode(pin_reset, INPUT);
  pinMode(pin_pwdn, OUTPUT);

  SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE1));
  
  process_it = false;
  SPI.attachInterrupt();
  SPDR = 0;

  read_reg = false;
  write_reg = false;
  proc_wreg = false;
  rw_done = true;
  base_addr = 0x00;
  rw_offset = 0;
  n_regs = 0;


  current_state = "RDATAC";
  delay(1000);
  digitalWrite(pin_pwdn, HIGH);
}

void loop() {
  // Do nothing for now, ISR takes too long to interrupt Serial 
}

// Depending on the command recieved from the Pi, send a known register value back to the Pi.
// Only process Data Read commands and Register Read Commands 
//  Data Read commands (single byte)
//  RDATAC = 0b0001_0000
//  SDATAC = 0b0001_0001
//  RDATA  = 0b0001_0010
// 
//  Register Read Commands
//  RREG   = 0b001r_rrrr 0b000n_nnnn
//  WREG   = 0b010r_rrrr 0b000n_nnnn
//
// TODO: Add support for System Commands: WAKEUP, STANDBY, RESET, START, STOP
ISR(SPI_STC_vect) {
  rec_byte = SPDR;

  if (!read_reg && !write_reg) {
    SPDR = 0x00;
    if (((rec_byte >> 4) == 0x1) && !(rec_byte & 0xE6)) {        // Is a Data Read command
      opcode = rec_byte & 0xF; // Isolate 4 lower bits
      if (opcode == 0x0)
        current_state = "RDATAC";
      else if (opcode == 0x1)
        current_state = "SDATAC";
      else if (opcode == 0x2)
        current_state = "RDATA";
      else 
        current_state = "NULL_STATE";
      process_it = true;
    } else if ((rec_byte >> 5) == 0x1) { // Is a RREG command
      read_reg = true;
      base_addr = rec_byte & 0x1F;
      rw_offset = 0;
    } else if ((rec_byte >> 5) == 0x2) { // Is a WREG command
      write_reg = true;
      base_addr = rec_byte & 0x1F;
      rw_offset = 0;
    }
    process_it = true;
  } else {                               // Previous byte was a RREG or WREG command
      if (rw_done) {                     // If read_reg or write_reg are true and rw_done is high, this means we have not shifted any data out yet
        n_regs = rec_byte & 0x1F;        // Read in the number of registers to read (second byte in RREG/WREG command
        rw_done = false;
      } 
      if (read_reg) {
        SPDR = regs_defaults[base_addr + rw_offset]; // Put register data on SPDR, so next SPI transfer from the master will shift out the data
        rw_offset++;
      } else if (write_reg) {
        if (proc_wreg) {                // proc_wreg will be false if we just shifted in the second byte of the WREG command.
          regs_defaults[base_addr + rw_offset] = rec_byte;
          rw_offset++;
        } else {
          proc_wreg = true;             // Only process SPDR from the Pi if data is REG_DATA
        }
      }
      

      if (rw_offset > n_regs) {          // Finished reading registers, reset all flags to defaults and set rw_done high
        read_reg = false;
        write_reg = false;
        proc_wreg = false;
        rw_done = true;
        base_addr = 0x00;
        rw_offset = 0;  
        n_regs = 0;
      }
  }
}


