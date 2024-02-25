/**
 * @brief A* algorthm that will find the best path between 2 points based on a weight map
 * @author phun
 * @date 2/8/24
 */

#include "PQueue.h"
#include "Config.h"

#ifndef ASTAR_H
#define ASTAR_H

float as_calcDistCost(int currX, int currY, int endX, int endY);
int as_isValid(int x, int y, int width, int length);
path_t* aStar(int grid[WIDTH][LENGTH], int width, int length, int startX, int startY, int endX, int endY, float biomeFactor, int neighbors);
path_t* ConstructPath(int cameFrom[WIDTH * LENGTH], int current, int start);
int PrintWeightMap(float map[WIDTH * LENGTH], int startX, int startY);

#endif