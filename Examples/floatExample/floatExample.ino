#include "fourDigit.h"

void setup() {
  Serial.begin(9600);
  initDigits();
}

void loop() {
  float valueSerial = 0;
  while (1) {
    if (Serial.available() > 0) {
      valueSerial = Serial.parseFloat();
      Serial.print("Recieved: ");
      Serial.println(valueSerial, 3);
    }
    setFloatDigits(valueSerial);
  }
}

