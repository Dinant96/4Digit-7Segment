//Author: Dinant Reijnders
#ifndef FOURDIGIT_h
#define FOURDIGIT_h

#define NUMBER_OF_DIGITS 4

//------------------- define ports -------------------

//              10  9   4   11    12   8
//             _|_  |   |  _|_   _|_   |
//            |   | |   | |   | |   |  |
//            |   | |   | |   | |   |  |
//            |   | |   | |   | |   |  |
//            |___| |   | |___| |___| /
//              |   |   |   |    /   /
//              D1  A   F   D2  D3  B
//     _________|___|___|___|___|___|_________
//    |   ___       ___       ___       ___   |
//    | |     |   |     |   |     |   |     | |
//    | | ___ |   | ___ |   | ___ |   | ___ | |
//    | |     |   |     |   |     |   |     | |
//    | | ___ |.  | ___ |.  | ___ |.  | ___ |.|
//    |_______________________________________|
//              |   |   |   |   |   |
//              E   D   DP  C   G   D4
//              |   |   |   |   |  _|_
//              |   |   |   |   | |   |
//              |   |   |   |   | |   |
//              |   |   |   |   | |   |
//              |   |   |   |   | |___|
//              |   |   |   |   |   |
//              5   6   2   7   3   13

#define Display_4 13
#define Display_3 12
#define Display_2 11
#define Display_1 10

#define A 9
#define B 8
#define C 7
#define D 6
#define E 5
#define F 4
#define G 3
#define DP 2

//---------------------------------------------------

//--------------- define clock settings -------------
#define ENABLE_CLOCK    0x01
#define DISABLE_CLOCK   0x02

#define COUNT_UP        0x04
#define COUNT_DOWN      0x08

#define HOUR_MINUTE     0x10
#define MINUTE_SECONDS  0x20

//Example:  initClock(ENABLE_CLOCK | COUNT_UP | MINUTE_SECONDS, 12, 34, 52);
//Example:  setSettingsClock(ENABLE_CLOCK | COUNT_UP | MINUTE_SECONDS);
//---------------------------------------------------

void initDigits(void);          //set this function always in the setup otherwise the digts won't turn on

void initClock(int16_t settings, int8_t hour, int8_t minute, int8_t second);//set this function always in the setup when using the clock
void setSettingsClock(int16_t settings);
void set_clock(int8_t hour, int8_t minute, int8_t second);
void get_clock(int8_t *phour, int8_t *pminute, int8_t *psecond);
void clockHandler(void);        //when using clock, call this function always in the loop

void setFloatDigits(float value);

void setIntDigits(int16_t value);
void setUIntDigits(uint16_t value);

#endif
