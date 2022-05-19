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

const char zoom = 5;  //size of a cell in map (keep it odd because of the player arrow size)

extern struct cell playerPos;
extern char exitPosX;
extern char menuPosition;
extern char pausePosition;
extern const char *menuItems[];
extern const char *pauseItems[];
extern const char *exitItems[];
extern bool mapUnlocked;
extern bool mapLockedTrigger;
extern char helpPosition;
extern String versionArduMaze;
extern char exitPosition;
extern char wheelMenuPosition;
extern char optionsPosition;
extern char showGrid;

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
void drawMapUnlocked();
void drawExitGame();
void drawEndScreen();
void drawPause();
#endif
