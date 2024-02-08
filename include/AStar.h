#include "PQueue.h"
#include "Config.h"

#ifndef ASTAR_H
#define ASTAR_H

float as_calcDistCost(int currX, int currY, int endX, int endY);
int as_isValid(int x, int y, int start, int width, int length);
path_t* aStar(int** grid, int width, int length, int startX, int startY, int endX, int endY, pqueue_t* open);
path_t* ConstructPath(int cameFrom[WIDTH * LENGTH], int current, int start);

#endif