//Author: Dinant Reijnders

#include "Arduino.h"
#include "fourDigit.h"
#include <TimerOne.h>

#define TIMER_US 2500    // 2.5mS set timer duration in microseconds 

//Local defines and functions
#define MINUS_SIGN 16
#define DIGIT_OFF 17
#define ERROR_SIGN 99

void set_display(uint8_t select, uint8_t digit);
bool clockTimer(void);
void setDigitsClock(int8_t hour, int8_t minute, int8_t second);

//Variables
int digitCounter = 0;
uint16_t DIGIT[NUMBER_OF_DIGITS];
uint16_t REST;
uint16_t tickCounter = 0;
bool secondflag = false;

int8_t secondeclock = 0;
int8_t minuteclock = 0;
int8_t hourclock = 0;

bool clockOnOff = false;
bool clockCountUpDown = false;
bool clockTimeFrame = false;

void timerIsr() {
  tickCounter++;
  set_display(digitCounter, DIGIT[digitCounter]);
  if (++digitCounter == NUMBER_OF_DIGITS) digitCounter = 0;
}

void initDigits(void) {
  pinMode(Display_4, 1);
  pinMode(Display_3, 1);
  pinMode(Display_2, 1);
  pinMode(Display_1, 1);

  pinMode(A, 1);
  pinMode(B, 1);
  pinMode(C, 1);
  pinMode(D, 1);
  pinMode(E, 1);
  pinMode(F, 1);
  pinMode(G, 1);
  pinMode(DP, 1);

  Timer1.initialize(TIMER_US);                  // Initialise timer 1
  Timer1.attachInterrupt( timerIsr );           // attach the ISR routine here
  setUIntDigits(0);
}

void initClock(int16_t settings, int8_t hour, int8_t minute, int8_t second) {
  //Check settings
  if ((ENABLE_CLOCK & settings) != 0)   clockOnOff = true;
  if ((DISABLE_CLOCK & settings) != 0)  clockOnOff = false;
  if ((COUNT_UP & settings) != 0)       clockCountUpDown = true;
  if ((COUNT_DOWN & settings) != 0)     clockCountUpDown = false;
  if ((HOUR_MINUTE & settings) != 0)    clockTimeFrame = true;
  if ((MINUTE_SECONDS & settings) != 0) clockTimeFrame = false;

  hourclock = hour;
  minuteclock = minute;
  secondeclock = second;
}

void setSettingsClock(int16_t settings) {
  //Check settings
  if ((ENABLE_CLOCK & settings) != 0)   clockOnOff = true;
  if ((DISABLE_CLOCK & settings) != 0)  clockOnOff = false;
  if ((COUNT_UP & settings) != 0)       clockCountUpDown = true;
  if ((COUNT_DOWN & settings) != 0)     clockCountUpDown = false;
  if ((HOUR_MINUTE & settings) != 0)    clockTimeFrame = true;
  if ((MINUTE_SECONDS & settings) != 0) clockTimeFrame = false;
}

void set_clock(int8_t hour, int8_t minute, int8_t second) {
  hourclock = hour;
  minuteclock = minute;
  secondeclock = second;
  tickCounter = 0;
}

void get_clock(int8_t *phour, int8_t *pminute, int8_t *psecond) {
  *phour = hourclock;
  *pminute = minuteclock;
  *psecond = secondeclock;
}

void clockHandler(void) {
  if (clockTimer() && clockOnOff) {
    if (clockCountUpDown) {
      if (++secondeclock == 60) {
        secondeclock = 0;
        if (++minuteclock == 60) {
          minuteclock = 0;
          if (++hourclock == 24) hourclock = 0;
        }
      }
    } else {
      if (secondeclock-- == 0) {
        secondeclock = 59;
        if (minuteclock-- == 0) {
          minuteclock = 59;
        }
        if (hourclock-- == 0) hourclock = 23;
      }
    }
    setDigitsClock(hourclock, minuteclock, secondeclock);
  }
}

bool clockTimer(void) {
  //clock runs 2.41669535560669 minutes late after 24 hours by 395
  if (tickCounter >= 400) {//ideal: 395,3318745441209//ideal: 404,6681254558791
    tickCounter -= 400;
    return true;
  }
  return false;
}

void setDigitsClock(int8_t hour, int8_t minute, int8_t second) {
  if (clockTimeFrame) {

    //Hour minute frame
    if (hour >= 10) {
      DIGIT[0] = hour / 10;
    } else {
      DIGIT[0] = DIGIT_OFF;
    }
    DIGIT[1] = hour % 10;
    DIGIT[2] = minute / 10;
    DIGIT[3] = minute % 10;

  } else {

    //Minute secnd frame
    if (minute >= 10) {
      DIGIT[0] = minute / 10;
    } else {
      DIGIT[0] = DIGIT_OFF;
    }
    DIGIT[1] = minute % 10;
    DIGIT[2] = second / 10;
    DIGIT[3] = second % 10;
  }

  //Blink dot very second
  secondflag = !secondflag;
  if (secondflag) {
    DIGIT[1] += 128;
  }
}

void setFloatDigits(float value) {
  uint8_t dotPosition = 3;
  REST = value;
  if (value < 10) {
    REST = value * 1000;
    dotPosition = 0;
  } else if (value < 100) {
    REST = value * 100;
    dotPosition = 1;
  } else if (value < 1000) {
    REST = value * 10;
    dotPosition = 2;
  }
  DIGIT[0] = REST / 1000;
  REST  = REST % 1000;
  DIGIT[1] = REST / 100;
  REST  = REST % 100;
  DIGIT[2] = REST / 10;
  DIGIT[3] = REST % 10;
  if (dotPosition < 3)
    DIGIT[dotPosition] += 128;//place dot
}

void setIntDigits(int16_t value) {
  int count = 0;
  bool negative = false;

  if (value < 0) {
    negative = true;
    REST  = value * -1;
    ++count;
  } else {
    REST = value;
  }

  int16_t n = value;
  while (n != 0) {
    n /= 10;
    ++count;
  }

  switch (count) {

    case 4:
      if (negative) {
        DIGIT[0] = MINUS_SIGN;
      } else {
        DIGIT[0] = REST / 1000;
        REST  = REST % 1000;
      }
      DIGIT[1] = REST / 100;
      REST  = REST % 100;
      DIGIT[2] = REST / 10;
      DIGIT[3] = REST % 10;
      break;

    case 3:
      DIGIT[0] = DIGIT_OFF;

      if (negative) {
        DIGIT[1] = MINUS_SIGN;
      } else {
        DIGIT[1] = REST / 100;
        REST  = REST % 100;
      }

      DIGIT[2] = REST / 10;
      DIGIT[3] = REST % 10;
      break;

    case 2:
      DIGIT[0] = DIGIT_OFF;
      DIGIT[1] = DIGIT_OFF;

      if (negative) {
        DIGIT[2] = MINUS_SIGN;
      } else {
        DIGIT[2] = REST / 10;
      }

      DIGIT[3] = REST % 10;
      break;

    case 1:
    case 0:
      DIGIT[0] = DIGIT_OFF;
      DIGIT[1] = DIGIT_OFF;
      DIGIT[2] = DIGIT_OFF;
      DIGIT[3] = REST;
      break;

    default:
      DIGIT[0] = MINUS_SIGN;
      DIGIT[1] = MINUS_SIGN;
      DIGIT[2] = MINUS_SIGN;
      DIGIT[3] = MINUS_SIGN;
      break;
  }
}

void setUIntDigits(uint16_t value) {
  setIntDigits(value);
  /*uint16_t n = value;
  int count = 0;
  while (n != 0) {
    n /= 10;
    ++count;
  }
  REST = value;
  switch (count) {

    case 4:
      DIGIT[0] = REST / 1000;
      REST  = REST % 1000;
      DIGIT[1] = REST / 100;
      REST  = REST % 100;
      DIGIT[2] = REST / 10;
      DIGIT[3] = REST % 10;
      break;

    case 3:
      DIGIT[0] = DIGIT_OFF;
      DIGIT[1] = REST / 100;
      REST  = REST % 100;
      DIGIT[2] = REST / 10;
      DIGIT[3] = REST % 10;
      break;

    case 2:
      DIGIT[0] = DIGIT_OFF;
      DIGIT[1] = DIGIT_OFF;
      DIGIT[2] = REST / 10;
      DIGIT[3] = REST % 10;
      break;

    case 1:
    case 0:
      DIGIT[0] = DIGIT_OFF;
      DIGIT[1] = DIGIT_OFF;
      DIGIT[2] = DIGIT_OFF;
      DIGIT[3] = REST;
      break;

    default:
      DIGIT[0] = MINUS_SIGN;
      DIGIT[1] = MINUS_SIGN;
      DIGIT[2] = MINUS_SIGN;
      DIGIT[3] = MINUS_SIGN;
      break;
  }*/
}

void set_display(uint8_t select, uint8_t digit) {
  bool dot =  false;
  if (digit & 0b10000000) {
    dot = true;
    digit -= 128;
  }

  switch (digit)
  {
    case 0:
      digit = 0b00111111;
      break;
    case 1:
      digit = 0b00000110;
      break;
    case 2:
      digit = 0b01011011;
      break;
    case 3:
      digit = 0b01001111;
      break;
    case 4:
      digit = 0b01100110;
      break;
    case 5:
      digit = 0b01101101;
      break;
    case 6:
      digit = 0b01111101;
      break;
    case 7:
      digit = 0b00000111;
      break;
    case 8:
      digit = 0b01111111;
      break;
    case 9:
      digit = 0b01101111;
      break;
    case 10:
      digit = 0b01110111;
      break;
    case 11:
      digit = 0b01111100;
      break;
    case 12:
      digit = 0b01011000;
      break;
    case 13:
      digit = 0b01011110;
      break;
    case 14:
      digit = 0b01111001;
      break;
    case 15:
      digit = 0b01110001;
      break;
    case MINUS_SIGN:
      digit = 0b01000000;//minus sign
      break;
    case DIGIT_OFF:
      digit = 0b00000000;//digit off
      break;
    default:
      digit = 0b01001001;//error
      break;
  }
  if (dot) digit |= 0b10000000;


  digitalWrite(Display_1, 1);
  digitalWrite(Display_2, 1);
  digitalWrite(Display_3, 1);
  digitalWrite(Display_4, 1);

  digitalWrite(A, digit & 0b00000001);
  digitalWrite(B, digit & 0b00000010);
  digitalWrite(C, digit & 0b00000100);
  digitalWrite(D, digit & 0b00001000);
  digitalWrite(E, digit & 0b00010000);
  digitalWrite(F, digit & 0b00100000);
  digitalWrite(G, digit & 0b01000000);
  digitalWrite(DP, digit & 0b10000000);

  switch (select)
  {
    case 0:
      digitalWrite(Display_1, 0);
      break;
    case 1:
      digitalWrite(Display_2, 0);
      break;
    case 2:
      digitalWrite(Display_3, 0);
      break;
    case 3:
      digitalWrite(Display_4, 0);
      break;
  }
}
