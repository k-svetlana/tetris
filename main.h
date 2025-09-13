#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void init_screen();
void draw();
void spawnPiece();
int canMove(int nx, int ny);

#endif