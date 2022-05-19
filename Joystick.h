#ifndef Joystick_h
#define Joystick_h

#include "Arduino.h"

//== Joystick pins ==//
#define X_PIN A0
#define Y_pin A1
#define Joy_switch 7

//== Joystick values ==// (representated by individual bits)
#define NONE 0    //deafult value for no input
#define SELECT 1  // 1. bit
#define UP 2      // 2. bit
#define DOWN 4    // 3. bit
#define LEFT 8    // 4. bit
#define RIGHT 16  // 5. bit
//example for input RIGHT and UP while button IS PRESSED: 00010011 = 2^0 + 2^1 + 2^5 = 19, so the output would be 19

char read_joystick();
void setupJoystick();

void waitForAnyInput();
void waitForNoInput();
void waitForSelect();
void waitForNoSelect();
#endif
