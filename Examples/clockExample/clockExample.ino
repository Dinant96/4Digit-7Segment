#include "fourDigit.h"

#define GET_CLOCK 1
#define SET_CLOCK 2

void setup() {
  Serial.begin(9600);
  initDigits();
  initClock(ENABLE_CLOCK | COUNT_UP | MINUTE_SECONDS, 0, 0, 0);
}

void loop() {
  int8_t valueSerial = 0;
  int8_t hour, minute, second;
  Serial.println("Give '1' for get_clock and '2' for set_clock");
  while (1) {

    clockHandler();
    
    if (Serial.available() > 0) {
      valueSerial = Serial.parseInt();
    }

    switch (valueSerial) {
      case GET_CLOCK:
        valueSerial = 0;

        get_clock(&hour, &minute, &second);
        Serial.println(hour, DEC);
        Serial.println(minute, DEC);
        Serial.println(second, DEC);
        break;
      case SET_CLOCK:
        valueSerial = 0;

        Serial.print("Give hour: ");
        while (Serial.available() == 0);
        hour = Serial.parseInt();
        Serial.println(hour, DEC);

        Serial.print("Give minute: ");
        while (Serial.available() == 0);
        minute = Serial.parseInt();
        Serial.println(minute, DEC);

        Serial.print("Give second: ");
        while (Serial.available() == 0);
        second = Serial.parseInt();
        Serial.println(second, DEC);

        set_clock(hour, minute, second);
        break;
    }
  }
}

