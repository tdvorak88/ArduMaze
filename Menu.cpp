//== This script creates the menu ==//
#include "Arduino.h"
#include "Menu.h"
#include "Joystick.h"
#include "Graphics.h"

char menuPosition;

const char *menuItems[] =
{
  "Start Game",
  "Options",
  "Help",
};

char doMenu() {
  waitForNoInput();
  menuPosition = 0;
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
    if (menuPosition > 2) {
      menuPosition = 2;
    }
    if (menuPosition < 0) {
      menuPosition = 0;
    }
    drawMenu();
    delay(200);
    waitForAnyInput();
  } while (1);
}

char pausePosition;

const char *pauseItems[] =
{
  "Resume",
  "Map",
  "Exit",
};

char doPause() {
  waitForNoInput();
  pausePosition = 0;
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
    if (pausePosition > 2) {
      pausePosition = 2;
    }
    if (pausePosition < 0) {
      pausePosition = 0;
    }
    drawPause();
    delay(200);
  } while (1);
}

char helpPosition = 1;

void doHelp() {
  waitForNoInput();
  helpPosition = 0;
  do {
    int input = read_joystick();
    switch (input) {
      case DOWN:
        helpPosition++;
        break;
      case UP:
        helpPosition--;
        break;
      case SELECT:
        return;
    }
    if (helpPosition > 2) {
      helpPosition = 2;
    }
    if (helpPosition < 1) {
      helpPosition = 1;
    }
    drawHelp();
    delay(100);
  } while (1);
}


const char *exitItems[] =
{
  "No",
  "Yes",
};

char exitPosition;

char doExitGame() {
  waitForNoInput();
  exitPosition = 0;
  do {
    int input = read_joystick();
    switch (input) {
      case RIGHT:
        exitPosition++;
        break;
      case LEFT:
        exitPosition--;
        break;
      case SELECT:
        return exitPosition;
    }
    if (exitPosition > 1) {
      exitPosition = 1;
    }
    if (exitPosition < 0) {
      exitPosition = 0;
    }
    drawExitGame();
    delay(100);
  } while (1);
}

char optionsPosition;
char showGrid = 1;

char doOptions() {
  waitForNoInput();
  optionsPosition = 0;
  do {
    int input = read_joystick();
    switch (input) {
      case UP:
        optionsPosition--;
        break;
      case DOWN:
        optionsPosition++;
        break;
      case SELECT:
        switch (optionsPosition) {
          case 0:
            if (mapUnlocked) {
              mapUnlocked = 0;
              break;
            } else {
              mapUnlocked = 1;
              break;
            }
          case 1:
            if (showGrid) {
              showGrid = 0;
              break;
            } else {
              showGrid = 1;
              break;
            }
          case 2:
            return;
        }
    }

    if (optionsPosition > 2) {
      optionsPosition = 2;
    }
    if (optionsPosition < 0) {
      optionsPosition = 0;
    }
    drawOptions();
    delay(100);
  } while (1);
}
