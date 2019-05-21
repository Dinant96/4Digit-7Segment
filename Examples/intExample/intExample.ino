#include "fourDigit.h"

void setup() {
  Serial.begin(9600);
  initDigits();
}

void loop() {
  int16_t valueSerial = 0;
  while (1) {
    if (Serial.available() > 0) {
      valueSerial = Serial.parseInt();
      Serial.print("Recieved: ");
      Serial.println(valueSerial, DEC);
    }
    setIntDigits(valueSerial);
  }
}

