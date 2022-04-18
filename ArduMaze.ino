//== Arduino Uno Maze Game ==//
//== Using U8g2 library by Olikraus ==//
//== Created by Tomas Dvorak ==//
//== 18.04.2022 ==//

#include "U8g2lib.h"
#include "Joystick.h"
#include "Menu.h"
#include "Graphics.h"
#include "MazeGenerator.h"

struct cell playerPos;
extern U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2;

void setup(void) {
  u8g2.begin();
  Serial.begin(9600);
  setupJoystick();
  randomSeed(analogRead(A5));
}

void loop(void) {
  switch (doMenu()) {
    case 0:
      startGame();
    case 1:
      drawOptions();
      break;
    case 2:
      drawHelp();
      break;
  }
  waitForNoInput();
  waitForAnyInput();
}
