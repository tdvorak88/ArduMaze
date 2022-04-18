//== This script creates the menu ==//
#include "Arduino.h"
#include "Menu.h"
#include "Joystick.h"
#include "Graphics.h"

char menuPosition = 0;
const char *menuItems[] =
{
  "Start Game",
  "Options",
  "Help",
};

char doMenu() {
  waitForNoInput();    //stops the menu on a selected option
  do {
    int input = read_joystick();
    switch (input) {
      case DOWN:
        menuPosition++;
        break;
      case UP:
        menuPosition--;
        break;
      case SELECT:
        return menuPosition;
    }
    if (menuPosition >= 2){
      menuPosition = 2;
    }
    if (menuPosition < 0){
      menuPosition = 0;
    }
    drawMenu();
    waitForAnyInput();
    delay(100);
  } while (1);
}

char pausePosition = 0;
const char *pauseItems[] =
{
  "Resume",
  "Help",
  "Exit",
};

char doPause() {
  waitForNoInput();    //stops the menu on a selected option
  do {
    int input = read_joystick();
    switch (input) {
      case DOWN:
        pausePosition++;
        break;
      case UP:
        pausePosition--;
        break;
      case SELECT:
        return pausePosition;
    }
    if (pausePosition >= 2) {
      pausePosition = 2;
    }
    if (pausePosition < 0) {
      pausePosition = 0;
    }
    drawPause();
    delay(100);
  } while (1);
} 
