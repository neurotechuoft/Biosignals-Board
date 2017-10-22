#include <SPI.h>


void setup() {
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  digitalWrite(SCK, LOW);
  digitalWrite(MOSI, LOW);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  
}

void loop() {
  char c;

  digitalWrite(SS, LOW);

  for (const char *p = "Hellow, world!\n"; c = *p; p++) {
    SPI.transfer(c);
  }

  digitalWrite(SS, HIGH);

  delay(1000);
}
