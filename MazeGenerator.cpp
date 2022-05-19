//== This script generates the maze ==//
#include "Arduino.h"
#include "MazeGenerator.h"
#include "Menu.h"
#include "U8g2lib.h"
#include "Joystick.h"
#include "Graphics.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

char maze[MAXX][MAXY];
char startPosX;
char exitPosX;

struct cell mapPos;
struct cell pillar[plrsmax];

bool mapUnlocked = 0;
bool mapLockedTrigger = 0;
/**
   Fills maze with walls and empty space.
*/
void setupMaze() {
  for (int x = 0; x < MAXX; 2 * x++) { //creates empty cells
    for (int y = 0; y < MAXY; 2 * y++) {
      maze[x][y] = EMPTY;
    }
  }
  for (int x = 0; x < MAXX; x++) { //creates outer walls
    maze[x][0] = WALL;
    maze[x][MAXY - 1] = WALL;
    maze[0][x] = WALL;
    maze[MAXX - 1][x] = WALL;
  }
  int idx;
  for (int x = 2; x < MAXX - 1; x += 2) { //creates pillars
    for (int y = 2; y < MAXY - 1; y += 2) {
      maze[x][y] = PILLAR;      //fills the maze with the pillars
      pillar[idx].x = x;        //stores them in an array for future use
      pillar[idx].y = y;
      idx++;
    }
  }
  for (int plrs = plrsmax; plrs > 0; plrs--) {  //connects pillars and walls
    int idx = random(plrs);             //random pillar selection
    int dirplrs = random(4);            //random direction
    if (dirplrs == 0) {                  //up
      for (int y = pillar[idx].y; maze[pillar[idx].x][y] != WALL; y--) {
        maze[pillar[idx].x][y] = WALL;
      }
    }
    if (dirplrs == 1)  {                 //right
      for (int x = pillar[idx].x; maze[x][pillar[idx].y] != WALL; x++) {
        maze[x][pillar[idx].y] = WALL;
      }
    }
    if (dirplrs == 2)  {                 //down
      for (int y = pillar[idx].y; maze[pillar[idx].x][y] != WALL; y++) {
        maze[pillar[idx].x][y] = WALL;
      }
    }
    if (dirplrs == 3) {                  //left
      for (int x = pillar[idx].x; maze[x][pillar[idx].y] != WALL; x--) {
        maze[x][pillar[idx].y] = WALL;
      }
    }
    pillar[idx] = pillar[plrs - 1];
  }

  do {                                      //finds an empty cell for a map item
    mapPos.x = rand() % MAXX;
    mapPos.y = rand() % MAXY;
  } while (maze[mapPos.x][mapPos.y] != EMPTY);

  maze[mapPos.x][mapPos.y] = MAP;                                      //creates a map in maze

  startPosX = 2 * (rand() % (MAXX - 1) / 2) + 1; //random start (even only)
  maze[startPosX][MAXY - 1] = EMPTY;    //creates start in maze

  exitPosX;
  do {
    exitPosX = 2 * (rand() % (MAXX - 1) / 2) + 1; //random exit (even only)
  }
  while (exitPosX == startPosX);                   //prevents exit being an exact opposite of start
  maze[exitPosX][0] = EXIT;     //creates exit in maze

}

void startGame() {
  setupMaze();

  playerPos.x = startPosX;  //moves player to startPosX
  playerPos.y = MAXY - 1;  //moves player to startPosY
  
  mapLockedTrigger = 0;

  waitForNoInput();

  do {
    int input = read_joystick();
    switch (input) {
      case SELECT:    //Pauses the game
        switch (doPause()) {
          case 0:
            break;
          case 1:
            if (mapUnlocked) {
              drawMap();
            } else {
              if (!mapLockedTrigger) {    //this is used for temporary showing the "find the map" text
                mapLockedTrigger = 1;
              }
            }
            break;
          case 2:
            switch (doExitGame()) {
              case 0:
                break;
              case 1:
                return;
            }
            break;
        }
      case UP:    //Move Up
        switch (view) {
          case NORTH:
            playerPos.y--;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.y++;
              break;
            }
            break;
          case EAST:
            playerPos.x++;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.x--;
              break;
            }
            break;
          case SOUTH:
            playerPos.y++;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.y--;
              break;
            }
            break;
          case WEST:
            playerPos.x--;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.x++;
              break;
            }
            break;
        }
        break;
      case RIGHT:   //Look Right
        view++;
        view = view % 4;
        break;
      case LEFT:    //Look Left
        view--;
        if (view < NORTH) {
          view = WEST;
          break;
        }
        break;
      case DOWN:    //Move Down
        switch (view) {
          case NORTH:
            playerPos.y++;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.y--;
              break;
            }
            break;
          case EAST:
            playerPos.x--;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.x++;
              break;
            }
            break;
          case SOUTH:
            playerPos.y--;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.y++;
              break;
            }
            break;
          case WEST:
            playerPos.x++;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.x--;
              break;
            }
            break;
        }
        break;

    }
    //drawSerial();
    draw3D();

    if (playerPos.x == mapPos.x && playerPos.y == mapPos.y) {
      if (!mapUnlocked) {
        drawMapUnlocked();
        mapUnlocked = 1;
      }
    }
    waitForAnyInput();
  } while (maze[playerPos.x][playerPos.y] !=  maze[exitPosX][0]);
  drawEndScreen();
  waitForNoInput();
  return;
}
