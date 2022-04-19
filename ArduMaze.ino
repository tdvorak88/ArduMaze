//== Arduino Uno Maze Game ==//
//== Using U8g2 library by Olikraus ==//
//== Created by Tomas Dvorak ==//
//== V1.1 ==//

/**
      V 1.1
      - added Map as an Item (Now you have to find it first)
      - updated Help screne (Now it has scrollable pages)
      - changed some fonts for exit and map (There's where the program got bigger)
      - fixed some render bugs
      - minor changes and optimizations of the code
*/

#include "U8g2lib.h"
#include "Joystick.h"
#include "Menu.h"
#include "Graphics.h"
#include "MazeGenerator.h"

String versionArduMaze = "V1.1";  //change every update!

struct cell playerPos;

extern U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2;

void setup(void) {
  u8g2.begin();
  //Serial.begin(9600);
  setupJoystick();
  randomSeed(analogRead(A5));   //make sure its not connected to anything!
}

void loop(void) {
  switch (doMenu()) {
    case 0:
      startGame();
      break;
    case 1:
      drawOptions();
      break;
    case 2:
      doHelp();
      break;
  }
}
