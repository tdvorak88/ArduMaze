//== This script processes joystick inputs ==//
#include "Arduino.h"
#include "joystick.h"

void setupJoystick() {
  pinMode(Joy_switch, INPUT_PULLUP);
}

char read_joystick() {
  char input = NONE;

  //== reads the joystick input ==//
  int X_Axis = analogRead(X_PIN);
  int Y_Axis = analogRead(Y_pin);
  int SW = digitalRead(Joy_switch);

  //== stores the inputs ==//
  if (SW == LOW) {
    input += SELECT;
  }
  if (X_Axis >= 800) {
    input += RIGHT;
  }
  if (X_Axis <= 200) {
    input += LEFT;
  }
  if (Y_Axis >= 800) {
    input += DOWN;
  }
  if (Y_Axis <= 200) {
    input += UP;
  }
  return input;
}

void waitForNoInput() {
  while (read_joystick() != NONE);
}

void waitForAnyInput() {
  while (read_joystick() == NONE);
}
void waitForSelect() {
  while (read_joystick() != SELECT);
}
void waitForNoSelect() {
  while (read_joystick() == SELECT);
}
