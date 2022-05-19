//== This script renders all the graphics using U8G2 Library ==//
#include "Arduino.h"
#include "Graphics.h"
#include "U8g2lib.h"
#include "Joystick.h"
#include "MazeGenerator.h"
#include "Menu.h"

//== Display ==//
U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R2 /*R=rotation*/, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);


int moveMapX;
int moveMapY;   //used for moving the map on the screen

void setFontDefault() {
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.setFontPosBaseline();
  u8g2.setFontDirection(0);
  u8g2.setFontRefHeightText();
  u8g2.setDrawColor(1);
}

void drawMenu() {
  setFontDefault();
  u8g2.clearBuffer();

  u8g2.drawLine(0, 0, u8g2.getDisplayWidth(), 0);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("ArduMaze XXX")) / 2, ROW - 3); // (center of the screen, ROW + Title)
  u8g2.println("ArduMaze ");
  u8g2.println(versionArduMaze);
  u8g2.drawLine(0, ROW - 2, u8g2.getDisplayWidth(), ROW - 2);

  for (byte m = 0; m < 3; m++)  {   //m = number of menu items
    if (m == menuPosition) {    //only for selected menu item
      u8g2.drawRFrame(0, (m + 1) * ROW , u8g2.getDisplayWidth(), ROW, 3); //(x, y + Title, w , h, r), draws the cursor
    }
    u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth(menuItems[m])) / 2, (m + 1) * ROW + 9); // (center of the screen, ROW + Title)
    u8g2.println( menuItems[m]);
  }
  u8g2.sendBuffer();
}

void drawOptions()  {
  waitForNoInput();
  setFontDefault();
  u8g2.clearBuffer();

  u8g2.drawLine(0, 0, u8g2.getDisplayWidth(), 0);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("Options")) / 2, ROW - 3);
  u8g2.println("Options");
  u8g2.drawLine(0, ROW - 2, u8g2.getDisplayWidth(), ROW - 2);

  for (byte o = 0; o < 3; o++)  {
    if (o == optionsPosition && o != 2) {
      u8g2.drawRFrame(3 * (u8g2.getDisplayWidth() / 4) - 1, ROW + (o * ROW) - 1, 3 * ROW / 4 - 1, 3 * ROW / 4 - 1, 3);
    }
    if (o == optionsPosition && o == 2) {
      u8g2.drawRFrame((u8g2.getDisplayWidth() - u8g2.getStrWidth("EXIT")) / 2 - 3, 10 * ROW / 3 - 1, u8g2.getStrWidth("EXIT") + 6, 9, 3);
    }
  }
  u8g2.setFont(u8g2_font_u8glib_4_tr);

  u8g2.setCursor(0, 3 * ROW / 2);
  u8g2.println("UNLOCK MAP");

  if (mapUnlocked) {
    u8g2.drawRBox(3 * (u8g2.getDisplayWidth() / 4), ROW, ROW / 2, ROW / 2, 2);
  } else {
    u8g2.drawRFrame(3 * (u8g2.getDisplayWidth() / 4), ROW, ROW / 2, ROW / 2, 2);
  }

  u8g2.setCursor(0, 5 * ROW / 2);
  u8g2.println("SHOW GRID");

  if (showGrid) {
    u8g2.drawRBox(3 * (u8g2.getDisplayWidth() / 4), 2 * ROW, ROW / 2, ROW / 2, 2);
  } else {
    u8g2.drawRFrame(3 * (u8g2.getDisplayWidth() / 4), 2 * ROW, ROW / 2, ROW / 2, 2);
  }

  setFontDefault();
  u8g2.drawLine(0, 3 * ROW + 2, 83, 3 * ROW + 2);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("EXIT")) / 2, 4 * ROW - 1);
  u8g2.println("EXIT");

  u8g2.sendBuffer();
}

void drawHelp()  {
  switch (helpPosition) {   //enables scrolling pages in the Help screne
    case 1:
      setFontDefault();
      u8g2.clearBuffer();

      u8g2.drawLine(0, 0, u8g2.getDisplayWidth(), 0);
      u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("Help")) / 2, ROW - 3); // (center of the screen, ROW + Title)
      u8g2.println("Help");
      u8g2.drawLine(0, ROW - 2, u8g2.getDisplayWidth(), ROW - 2);

      u8g2.setFont(u8g2_font_u8glib_4_tr);

      u8g2.setCursor(0, 3 * ROW / 2);
      u8g2.println("CONTROLS:");

      u8g2.setCursor(0, 4 * ROW / 2);
      u8g2.println("UP = WALK");

      u8g2.setCursor(0, 5 * ROW / 2);
      u8g2.println("DOWN = MAP");

      u8g2.setCursor(0, 6 * ROW / 2);
      u8g2.println("LEFT and RIGHT =");

      u8g2.setCursor(0, 7 * ROW / 2);
      u8g2.println("LOOK AROUND");

      u8g2.setCursor(0, 8 * ROW / 2);
      u8g2.println("SELECT = PAUSE");


      u8g2.drawLine(83, ROW, 83, ROW + (u8g2.getDisplayHeight() - ROW) / 2 - 1);
      u8g2.drawLine(82, ROW, 82, ROW + (u8g2.getDisplayHeight() - ROW) / 2 - 1);    //Scroll bar

      u8g2.sendBuffer();
      setFontDefault();
      break;

    case 2:
      setFontDefault();
      u8g2.clearBuffer();


      u8g2.drawLine(0, 0, u8g2.getDisplayWidth(), 0);
      u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("Help")) / 2, ROW - 3); // (center of the screen, ROW + Title)
      u8g2.println("Help");
      u8g2.drawLine(0, ROW - 2, u8g2.getDisplayWidth(), ROW - 2);

      u8g2.setFont(u8g2_font_u8glib_4_tr);    //change this to more readable font



      u8g2.setCursor(0, 3 * ROW / 2);
      u8g2.println("GOAL:");

      u8g2.setCursor(0, 4 * ROW / 2);
      u8g2.println("FIND THE EXIT");

      u8g2.setCursor(0, 5 * ROW / 2);
      u8g2.println("OR THE MAP");

      u8g2.setCursor(0, 6 * ROW / 2);
      u8g2.println("");

      u8g2.setCursor(0, 7 * ROW / 2);
      u8g2.println("WHICHEVER COMES");

      u8g2.setCursor(0, 8 * ROW / 2);
      u8g2.println("FIRST");

      u8g2.drawLine(83, ROW + (u8g2.getDisplayHeight() - ROW) / 2, 83, u8g2.getDisplayHeight());
      u8g2.drawLine(82, ROW + (u8g2.getDisplayHeight() - ROW) / 2, 82, u8g2.getDisplayHeight());  //Scroll bar

      u8g2.sendBuffer();
      setFontDefault();
      break;
  }
}

char view = NORTH;   //used in a cyclical change of view

/**
   Checks a location of a cell realtive to player view and returns its actual location in maze
   @arg x x-coordinate
   @arg y y-coordinate
   @returns what is on coordinate [x,y]
*/
char getViewedCell(char x, char y) {
  char a, b;
  switch (view) {
    case NORTH:
      a = x;
      b = (-y);
      break;
    case EAST:
      a = y;
      b = x;
      break;
    case SOUTH:
      a = (-x);
      b = y;
      break;
    case WEST:
      a = (-y);
      b = (-x);
      break;
  }
  if (playerPos.x + a > MAXX || playerPos.x + a < 0 || playerPos.y + b > MAXY || playerPos.y + b < 0) {   //treats all cells outside maze as empty
    return EMPTY;
  } else {
    return maze[playerPos.x + a][playerPos.y + b];
  }
}
/**
      draws the game into a serial monitor
*/
void drawSerial() {   //draws the game into a serial monitor
  Serial.print("\n");
  Serial.print("View: ");
  Serial.print(view);

  for (int y = 0; y < MAXY; y++) {
    for (int x = 0; x < MAXX; x++) {
      if (x == playerPos.x && y == playerPos.y) {
        Serial.print("PL");
      } else {
        if (x == exitPosX && y == 0) {
          Serial.print("FI");
        } else {
          if (maze[x][y] == WALL) {
            Serial.print("[]");
          }
          if (maze[x][y] == EMPTY) {
            Serial.print("  ");
          }
        }
      }
    }
    Serial.print("\n");
  }
}

/**
      draws the player as an arrow on the map
*/
void drawMapPlayer() {
  u8g2.setFont(u8g2_font_u8glib_4_tr);
  switch (view) {
    case NORTH:
      u8g2.setFontDirection(0);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 1 + moveMapX, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 5 + moveMapY, 94);    //"^"
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 2 + moveMapX, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 5 + moveMapY, 108);   //"l"
      break;
    case EAST:
      u8g2.setFontDirection(1);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 - 1 + moveMapX, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 1 + moveMapY, 94);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 - 1 + moveMapX, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 2 + moveMapY, 108);
      break;
    case SOUTH:
      u8g2.setFontDirection(2);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 3 + moveMapX, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 - 1 + moveMapY, 94);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 2 + moveMapX, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 - 1 + moveMapY, 108);
      break;
    case WEST:
      u8g2.setFontDirection(3);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 5 + moveMapX, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 3 + moveMapY, 94);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 5 + moveMapX, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 2 + moveMapY, 108);
      break;
  }
  u8g2.sendBuffer();
  setFontDefault();
}

/**
      draws the map
*/
void drawMap() {
  moveMapX = 0;
  moveMapY = 0;   //resets the map centering
  waitForNoInput();
  do {
    int input = read_joystick();
    switch (input) {
      case LEFT:
        moveMapX += 10;
        break;
      case RIGHT:
        moveMapX -= 10;
        break;
      case DOWN:
        moveMapY -= 10;
        break;
      case UP:
        moveMapY += 10;
        break;
      case SELECT:    //centers the map
        return;
    }
    u8g2.clearBuffer();
    for (int y = 0; y < MAXY; y++) {
      for (int x = 0; x < MAXX; x++) {
        if (maze[x][y] == WALL) {
          u8g2.drawFrame((zoom * x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + moveMapX, (zoom * y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + moveMapY, zoom, zoom);
        }
        if (maze[x][y] == EXIT) {
          u8g2.setFont(u8g2_font_u8glib_4_tr);
          u8g2.setCursor((zoom * x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + moveMapX, (zoom * y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 12 + moveMapY);
          u8g2.print("E");
        }
      }
    }
    u8g2.drawFrame(0, 0, 84, 48);   //displays border
    drawMapPlayer();

    u8g2.sendBuffer();
    waitForNoInput();
    waitForAnyInput();
  } while (1);
}

/**
      draws the grid used to later draw the whole scene
*/
void drawGrid() {
  u8g2.drawLine(0, 0, 31, 18);      //1.1 diagonal
  u8g2.drawLine(52, 29, 83, 47);    //1.2 digonal
  u8g2.drawLine(83, 0, 52, 18);     //2.1 diagonal
  u8g2.drawLine(31, 29, 0, 47);     //2.2 diagonal
  u8g2.drawFrame(0, 0, 84, 48);     //displays border
  u8g2.drawFrame(13, 7, 58, 34);    //1. frame
  u8g2.drawFrame(23, 13, 38, 22);   //2. frame
  u8g2.drawFrame(31, 18, 22, 12);   //3. frame

  u8g2.drawLine( 0, 7, 13, 7);      //1UL
  u8g2.drawLine( 0, 40, 13, 40);    //1DL
  u8g2.drawLine( 70, 7, 83, 7);     //1UR
  u8g2.drawLine( 70, 40, 83, 40);   //1DR
  u8g2.drawLine( 13, 13, 23, 13);   //2UL
  u8g2.drawLine( 13, 34, 23, 34);   //2DL
  u8g2.drawLine( 60, 13, 70, 13);   //2UR
  u8g2.drawLine( 60, 34, 70, 34);   //2DR
  u8g2.drawLine( 23, 18, 31, 18);   //3UL
  u8g2.drawLine( 23, 29, 31, 29);   //3DL
  u8g2.drawLine( 52, 18, 60, 18);   //3UR
  u8g2.drawLine( 52, 29, 60, 29);   //3DR

  u8g2.sendBuffer();
}

/**
      draws the game in 3D using the grid and getViewedCell
*/
void draw3D() {   //draws the 3D scene in the first person view
  u8g2.clearBuffer();
  if (showGrid) {
    drawGrid();
  }

  u8g2.setDrawColor(1);
  if (getViewedCell(0, 2) == EXIT) {
    u8g2.setFont(u8g2_font_4x6_mr);
    u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("EXIT")) / 2 + 1, (u8g2.getDisplayHeight()) / 2 + (ROW / 4) - 1);
    u8g2.println("EXIT");
  }

  if (getViewedCell(0, 1) == EXIT) {
    u8g2.setFont(u8g2_font_8x13_mr);
    u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("EXIT")) / 2, (u8g2.getDisplayHeight()) / 2 + (ROW / 2) - 2);
    u8g2.println("EXIT");
  }

  if (getViewedCell(0, 0) == EXIT) {
    u8g2.setFont(u8g2_font_helvR18_tr);
    u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("EXIT")) / 2, (u8g2.getDisplayHeight()) / 2 + ROW - 2);
    u8g2.println("EXIT");
  }
  setFontDefault();

  if (!mapUnlocked) {
    u8g2.setDrawColor(1);

    if (getViewedCell(0, 3) == MAP) {
      u8g2.setFont(u8g2_font_4x6_mr);
      u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("MAP")) / 2, (u8g2.getDisplayHeight()) / 2 + (ROW / 4) - 1);
      u8g2.println("MAP");
    }

    if (getViewedCell(0, 2) == MAP) {
      u8g2.setFont(u8g2_font_8x13_mr);
      u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("MAP")) / 2, (u8g2.getDisplayHeight()) / 2 + (ROW / 2) - 2);
      u8g2.println("MAP");
    }

    if (getViewedCell(0, 1) == MAP) {
      u8g2.setFont(u8g2_font_helvR18_tr);
      u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("MAP")) / 2, (u8g2.getDisplayHeight()) / 2 + ROW - 2);
      u8g2.println("MAP");
    }
    setFontDefault();
  }

  if (getViewedCell(0, 3) == WALL && getViewedCell(0, 3) != EXIT) {
    u8g2.drawBox(31, 18, 22, 12);   //3.Frame
  }

  if (getViewedCell(-1, 3) == WALL) {
    u8g2.drawBox(23, 18, 9, 12);
  }
  if (getViewedCell(1, 3) == WALL) {
    u8g2.drawBox(52, 18, 9, 12);
  }

  if (getViewedCell(0, 2) == WALL && getViewedCell(0, 2) != EXIT) {
    u8g2.drawBox(23, 13, 38, 22); //2.Frame
  }
  if (getViewedCell(-1, 2) == WALL) {
    u8g2.drawTriangle(23, 13, 23, 18, 32, 18);
    u8g2.drawBox(23, 18, 9, 12);
    u8g2.drawTriangle(23, 29, 32, 29, 23, 34);
    u8g2.drawBox( 13, 13, 11, 22);
  }
  if (getViewedCell(1, 2) == WALL) {
    u8g2.drawTriangle(52, 18, 60, 18, 60, 13);
    u8g2.drawBox(52, 18, 8, 12);
    u8g2.drawTriangle(52, 29, 60, 29, 60, 34);
    u8g2.drawBox(60, 13, 10, 22);
  }

  if (getViewedCell(0, 1) == WALL && getViewedCell(0, 1) != EXIT) {
    u8g2.drawBox(13, 7, 58, 34);  //1.frame
  }
  if (getViewedCell(-1, 1) == WALL) {
    u8g2.drawTriangle(13, 7, 13, 13, 23, 13);
    u8g2.drawBox(13, 13, 11, 22);
    u8g2.drawTriangle(13, 34, 23, 34, 13, 40);
    u8g2.drawBox( 0, 7, 13, 34 );
  }
  if (getViewedCell(1, 1) == WALL) {
    u8g2.drawTriangle(60, 13, 70, 13, 70, 7);
    u8g2.drawBox( 60, 13, 10, 22);
    u8g2.drawTriangle(60, 34, 70, 34, 70, 40);
    u8g2.drawBox(70, 7, 13, 34);
  }

  if (getViewedCell(-1, 0) == WALL) {
    u8g2.drawTriangle(0, 0, 13, 7, 0, 7);
    u8g2.drawBox(0, 7, 13, 34);
    u8g2.drawTriangle(0, 40, 13, 40, 0, 47);
  }
  if (getViewedCell(1, 0) == WALL) {
    u8g2.drawTriangle(83, 0, 70, 7, 83, 7);
    u8g2.drawBox(70, 7, 13, 34);
    u8g2.drawTriangle(70, 40, 83, 40, 83, 47);
  }

  /**
        compass
  */

  u8g2.setFont(u8g2_font_u8glib_4_tr);
  switch (view) {
    case NORTH:
      u8g2.setFontDirection(0);
      u8g2.drawGlyph(u8g2.getDisplayWidth() / 2, ROW / 2, 94);    //"^"
      u8g2.drawGlyph(u8g2.getDisplayWidth() / 2 + 1, ROW / 2, 108); //"l"
      break;
    case EAST:
      u8g2.setFontDirection(1);
      u8g2.drawGlyph(u8g2.getDisplayWidth() / 2, ROW / 2 - 3, 94);
      u8g2.drawGlyph(u8g2.getDisplayWidth() / 2, ROW / 2 - 2, 108);
      break;
    case SOUTH:
      u8g2.setFontDirection(2);
      u8g2.drawGlyph(u8g2.getDisplayWidth() / 2 + 2, 1, 94);
      u8g2.drawGlyph(u8g2.getDisplayWidth() / 2 + 1, 1, 108);
      break;
    case WEST:
      u8g2.setFontDirection(3);
      u8g2.drawGlyph(u8g2.getDisplayWidth() / 2 + 3, ROW / 2 - 1, 94);
      u8g2.drawGlyph(u8g2.getDisplayWidth() / 2 + 3, ROW / 2 - 2, 108);
      break;
  }
  u8g2.sendBuffer();
  setFontDefault();

  u8g2.setFont(u8g2_font_u8glib_4_tr);

  if (mapLockedTrigger && !mapUnlocked) {
    u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("FIND THE MAP")) / 2 , 4 * ROW - 2);
    u8g2.println("FIND THE MAP");

    mapLockedTrigger = 0;     //this ensures showing the the text only temporary
  }

  u8g2.sendBuffer();
  delay(300);    //refresh rate of the game
  setFontDefault();
}


void drawMapUnlocked() {
  u8g2.setFont(u8g2_font_u8glib_4_tr);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("MAP UNLOCKED")) / 2 , 4 * ROW - 2);
  u8g2.println("MAP UNLOCKED");
  u8g2.sendBuffer();
  setFontDefault();
}

void drawExitGame() {
  setFontDefault();
  u8g2.clearBuffer();

  u8g2.drawLine(0, 0, u8g2.getDisplayWidth(), 0);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("EXIT GAME")) / 2, ROW - 3); // (center of the screen, ROW + Title)
  u8g2.println("EXIT GAME");
  u8g2.drawLine(0, ROW - 2, u8g2.getDisplayWidth(), ROW - 2);

  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("ARE YOU SURE?")) / 2, 2 * ROW - 3); // (center of the screen, ROW + Title)
  u8g2.println("ARE YOU SURE?");

  for (byte e = 0; e < 2; e++)  {   //m = number of menu items
    if (e == exitPosition) {    //only for selected menu item
      u8g2.drawRFrame(e * (u8g2.getDisplayWidth() / 2) + ((u8g2.getDisplayWidth() / 2) - u8g2.getStrWidth("XXX")) / 2 - 2, 2 * ROW + (u8g2.getDisplayHeight() - ROW) / 2 - ROW, u8g2.getStrWidth("XXX") + 4, ROW, 3); //(x, y + Title, w , h, r), draws the cursor
    }
    u8g2.setCursor(e * (u8g2.getDisplayWidth() / 2) + ((u8g2.getDisplayWidth() / 2) - u8g2.getStrWidth("XXX")) / 2, 2 * ROW + (u8g2.getDisplayHeight() - ROW) / 2 - 3);
    u8g2.println( exitItems[e]);
  }
  u8g2.sendBuffer();
}

/**
      tells the player he found the exit and in how many steps, then waits for him to proceed
*/
void drawEndScreen() {
  setFontDefault();
  u8g2.clearBuffer();
  u8g2.drawLine(0, 0, u8g2.getDisplayWidth(), 0);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("Game Over")) / 2, ROW - 3); // (center of the screen, ROW + Title)
  u8g2.println("Game Over");
  u8g2.drawLine(0, ROW - 2, u8g2.getDisplayWidth(), ROW - 2);
  delay(1000);
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("click to continue...")) / 2, (u8g2.getDisplayHeight()) / 2 + (2 * ROW - 3));
  u8g2.println("click to continue...");
  u8g2.sendBuffer();

  setFontDefault();
  waitForSelect();
}

void drawPause() {
  setFontDefault();
  u8g2.clearBuffer();

  u8g2.drawLine(0, 0, u8g2.getDisplayWidth(), 0);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("Pause")) / 2, ROW - 3); // (center of the screen, ROW + Title)
  u8g2.println("Pause");
  u8g2.drawLine(0, ROW - 2, u8g2.getDisplayWidth(), ROW - 2);

  for (byte m = 0; m < 3; m++)  {   //m = number of menu items
    if (m == pausePosition) {    //only for selected menu item
      u8g2.drawRFrame(0, (m + 1) * ROW , u8g2.getDisplayWidth(), ROW, 3); //(x, y + Title, w , h, r), draws the cursor
    }
    u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth(pauseItems[m])) / 2, (m + 1) * ROW + 9); // (center of the screen, ROW + Title)
    u8g2.println( pauseItems[m]);
  }
  u8g2.sendBuffer();
}
