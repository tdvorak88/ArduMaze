//== This script renders all the graphics using U8G2 Library ==//
#include "Arduino.h"
#include "Graphics.h"
#include "U8g2lib.h"
#include "Joystick.h"
#include "MazeGenerator.h"
#include "Menu.h"

//== Display ==//
U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2(U8G2_R2 /*R=rotation*/, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

void setFontDefault() {
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.setFontPosBaseline();
  u8g2.setFontDirection(0);
  u8g2.setFontRefHeightText();
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

void drawOptions()  {   //TO DO: finish this
  waitForNoInput();

  setFontDefault();
  u8g2.clearBuffer();

  u8g2.drawLine(0, 0, u8g2.getDisplayWidth(), 0);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("Options")) / 2, ROW - 3);
  u8g2.println("Options");
  u8g2.drawLine(0, ROW - 2, u8g2.getDisplayWidth(), ROW - 2);

  u8g2.setFont(u8g2_font_u8glib_4_tr);
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("COMING IN NEXT PATCH")) / 2, 2 * ROW); // (center of the screen, ROW + Title)
  u8g2.println("COMING IN NEXT PATCH");

  u8g2.sendBuffer();
  setFontDefault();
  waitForSelect();    //should be in doOptions, if its made someday
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

      u8g2.setFont(u8g2_font_u8glib_4_tr);    //change this to more readable font

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


      u8g2.drawLine(83, ROW, 83, ROW + (u8g2.getDisplayHeight() - ROW) / 2);
      u8g2.drawLine(82, ROW, 82, ROW + (u8g2.getDisplayHeight() - ROW) / 2);    //Scroll bar

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
      u8g2.drawLine(82, ROW + (u8g2.getDisplayHeight() - ROW) / 2, 82, u8g2.getDisplayHeight());    //Scroll bar

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
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 1, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 5, 94);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 2, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 4, 108);
      break;
    case EAST:
      u8g2.setFontDirection(1);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 - 2, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 1, 94);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 - 1, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 2, 108);
      break;
    case SOUTH:
      u8g2.setFontDirection(2);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 2, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 - 2, 94);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 1, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 - 1, 108);
      break;
    case WEST:
      u8g2.setFontDirection(3);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 5, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 2, 94);
      u8g2.drawGlyph((zoom * playerPos.x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2 + 4, (zoom * playerPos.y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2 + 1, 108);
      break;
  }
  u8g2.sendBuffer();
  setFontDefault();
}

/**
      draws the map
*/
void drawMap() {
  u8g2.clearBuffer();
  for (int y = 0; y < MAXY; y++) {
    for (int x = 0; x < MAXX; x++) {
      if (maze[x][y] == WALL) {
        u8g2.drawFrame((zoom * x) + (u8g2.getDisplayWidth() - zoom * MAXX) / 2, (zoom * y) + (u8g2.getDisplayHeight() - zoom * MAXY) / 2, zoom, zoom);
      }
    }
  }
  u8g2.drawFrame(0, 0, 84, 48);   //displays border
  drawMapPlayer();
  u8g2.sendBuffer();
  waitForNoInput();
}

/**
      draws the grid used to later draw the whole scene
*/
void drawGrid() {   //TO DO: Split it into separate frames to better render exit
  u8g2.drawLine(0, 0, 32, 18);      //1.1 diagonal
  u8g2.drawLine(52, 29, 83, 47);    //1.2 digonal
  u8g2.drawLine(83, 0, 52, 18);     //2.1 diagonal
  u8g2.drawLine(32, 29, 0, 47);     //2.2 diagonal
  u8g2.drawFrame(0, 0, 84, 48);     //displays border
  u8g2.drawFrame(12, 7, 60, 34);    //1. frame
  u8g2.drawFrame(23, 13, 38, 22);   //2. frame
  u8g2.drawFrame(32, 18, 20, 12);   //3. frame
  u8g2.drawLine( 0, 7, 12, 7);      //1UL
  u8g2.drawLine( 0, 40, 12, 40);    //1DL
  u8g2.drawLine( 72, 7, 84, 7);     //1UR
  u8g2.drawLine( 72, 40, 84, 40);   //1DR
  u8g2.drawLine( 12, 13, 23, 13);   //2UL
  u8g2.drawLine( 12, 34, 23, 34);   //2DL
  u8g2.drawLine( 61, 13, 71, 13);   //2UR
  u8g2.drawLine( 61, 34, 71, 34);   //2DR
  u8g2.drawLine( 23, 18, 32, 18);   //3UL
  u8g2.drawLine( 23, 29, 32, 29);   //3DL
  u8g2.drawLine( 52, 18, 60, 18);   //3UR
  u8g2.drawLine( 52, 29, 60, 29);   //3DR
  u8g2.sendBuffer();
}

/**
      draws the game in 3D using the grid and getViewedCell
*/
void draw3D() {   //draws the 3D scene in the first person view
  u8g2.clearBuffer();
  drawGrid();

  setFontDefault();

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

  if (!mapUnlocked) {
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
  }

  if (getViewedCell(0, 3) == WALL && getViewedCell(0, 3) != EXIT) {
    u8g2.drawBox(32, 18, 20, 12);   //3.Frame
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
    u8g2.drawBox( 12, 13, 11, 22);
  }
  if (getViewedCell(1, 2) == WALL) {
    u8g2.drawTriangle(52, 18, 60, 18, 61, 13);
    u8g2.drawBox(52, 18, 8, 12);
    u8g2.drawTriangle(52, 29, 60, 29, 61, 34);
    u8g2.drawBox(61, 13, 10, 22);
  }

  if (getViewedCell(0, 1) == WALL && getViewedCell(0, 1) != EXIT) {
    u8g2.drawBox(12, 7, 60, 34);  //1.frame
  }
  if (getViewedCell(-1, 1) == WALL) {
    u8g2.drawTriangle(12, 7, 12, 13, 23, 13);
    u8g2.drawBox(12, 13, 12, 22);
    u8g2.drawTriangle(12, 34, 23, 34, 12, 40);
    u8g2.drawBox( 0, 7, 12, 34 );
  }
  if (getViewedCell(1, 1) == WALL) {
    u8g2.drawTriangle(61, 13, 71, 13, 72, 7);
    u8g2.drawBox( 61, 13, 10, 22);
    u8g2.drawTriangle(61, 34, 71, 34, 72, 40);
    u8g2.drawBox(72, 7, 12, 34);
  }

  if (getViewedCell(-1, 0) == WALL) {
    u8g2.drawTriangle(0, 0, 12, 7, 0, 7);
    u8g2.drawBox(0, 7, 12, 34);
    u8g2.drawTriangle(0, 40, 12, 40, 0, 47);
  }
  if (getViewedCell(1, 0) == WALL) {
    u8g2.drawTriangle(83, 0, 72, 7, 84, 7);
    u8g2.drawBox(72, 7, 12, 34);
    u8g2.drawTriangle(72, 40, 84, 40, 83, 47);
  }

  u8g2.setFont(u8g2_font_u8glib_4_tr);
  /*
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("SCORE:  ") - u8g2.getStrWidth("LEVEL  ")) / 2 - 1, ROW / 2);
  u8g2.println("SCORE: ");
  u8g2.println(score);
  
  u8g2.setCursor(u8g2.getDisplayWidth() / 2 + 3, ROW / 2);
  u8g2.println("LEVEL ");
  u8g2.println(level);
  */

  if (mapUnlocked) {
    u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("MAP UNLOCKED")) / 2 , 4 * ROW - 2);
    u8g2.println("MAP UNLOCKED");
  }

  if (mapLockedTrigger && !mapUnlocked) {
    u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("FIND THE MAP")) / 2 , 4 * ROW - 2);
    u8g2.println("FIND THE MAP");

    mapLockedTrigger = 0;     //this ensures showing the the text only temporary
  }

  u8g2.sendBuffer();
  delay(300);    //refresh rate of the game
  setFontDefault();
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
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("Your score is")) / 2, (u8g2.getDisplayHeight()) / 2);
  u8g2.println("Your score is ");
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth("XX steps")) / 2, (u8g2.getDisplayHeight()) / 2 + ROW);
  u8g2.println(score);
  u8g2.println(" points");
  u8g2.sendBuffer();

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
