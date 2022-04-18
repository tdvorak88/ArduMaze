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
struct cell pillar[plrsmax];
int steps;
int rooms = 1;

/**
   Fills maze with walls and empty space.
*/
void setupMaze() {
  for (int x = 0; x < MAXX; 2 * x++) { //creates empty cells
    for (int y = 0; y < MAXY; 2 * y++) {
      maze[x][y] = EMPTY;
    }
  }
  for (int x = 0; x < MAXX; x++) { //creates outer row walls
    maze[x][0] = WALL;
    maze[x][MAXY - 1] = WALL;
  }
  for (int y = 0; y < MAXY; y++) { //creates outer column walls
    maze[0][y] = WALL;
    maze[MAXX - 1][y] = WALL;
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
  playerPos.y = MAXY - 1;  //moves player to startPosY (MAXY-1)
  waitForNoInput();
  do {
    int input = read_joystick();
    switch (input) {
      case DOWN:
        draw2D();
        waitForNoInput();
        break;
      case UP:
        switch (view) {
          case NORTH:
            playerPos.y--;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.y++;
              break;
            }
            steps++;
            break;
          case EAST:
            playerPos.x++;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.x--;
              break;
            }
            steps++;
            break;
          case SOUTH:
            playerPos.y++;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.y--;
              break;
            }
            steps++;
            break;
          case WEST:
            playerPos.x--;
            if (maze[playerPos.x][playerPos.y] == WALL) {
              playerPos.x++;
              break;
            }
            steps++;
            break;
        }
        break;
      case RIGHT:
        view++;
        view = view % 4;
        break;
      case LEFT:
        view--;
        if (view < NORTH) {
          view = WEST;
          break;
        }
        break;
      case SELECT:
        switch (doPause()) {
          case 0:
            break;
          case 1:
            drawHelp();
            waitForNoInput();
            waitForAnyInput();
            waitForNoInput();
            break;
          case 2:
            return;
        }
        break;
    }

    if (maze[playerPos.x][playerPos.y] ==  maze[exitPosX][0]) {
      if (rooms < 5) {
        rooms++;
        startGame();
      } else {
        break;
      }
    }
    //drawSerial();
    draw3D();
    waitForAnyInput();
  } while (rooms <= 5);
  drawEndScreen();
  return;
}
