#ifndef MazeGenerator_h
#define MazeGenerator_h

#include "U8g2lib.h"
#include "Arduino.h"

#define MAXX 11
#define MAXY 11
/**
      both must be odd numbers because of the maze generation
*/

//== Cell Values ==//
#define WALL -1
#define EMPTY -2
#define PILLAR -3
#define EXIT -4

extern struct cell playerPos;
extern char maze[MAXX][MAXY];
extern char view;

struct cell {
  char x;
  char y;
};

const char zoom = 4;  //size of a cell in pixels
const char plrsmax = ((MAXX - 3) / 2) * ((MAXY - 3) / 2);

void setupMaze();
void startGame();
#endif
