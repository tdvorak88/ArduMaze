#ifndef Graphics_h
#define Graphics_h

#include "Arduino.h"
#include "U8g2lib.h"
#include "MazeGenerator.h"

//== View Values ==//
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

extern struct cell playerPos;
extern char exitPosX;
extern int steps;
extern char menuPosition;
extern char pausePosition;
extern const char *menuItems[];
extern const char *pauseItems[];
extern int rooms;

void setFontDefault();
void drawMenu();
void drawOptions();
void drawHelp();
char getViewedCell();
void drawSerial();  //only for debugging
void draw2DPlayer();
void draw2D();
void drawGrid();
void draw3D();
void drawEndScreen();
void drawPause();
#endif
