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
extern int score;
extern char menuPosition;
extern char pausePosition;
extern const char *menuItems[];
extern const char *pauseItems[];
extern int level;
extern bool mapUnlocked;
extern bool mapLockedTrigger;
extern char helpPosition;
extern String versionArduMaze;

void setFontDefault();
void drawMenu();
void drawOptions();
void drawHelp();
char getViewedCell();
void drawSerial();  //only for debugging
void drawMapPlayer();
void drawMap();
void drawGrid();
void draw3D();
void drawEndScreen();
void drawPause();
#endif
