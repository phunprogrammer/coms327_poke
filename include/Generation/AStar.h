/**
 * @brief A* algorthm that will find the best path between 2 points based on a weight map
 * @author phun
 * @date 2/8/24
 */

#include "Config.h"

#ifndef ASTAR_H
#define ASTAR_H

float as_calcDistCost(int currX, int currY, int endX, int endY);
int as_isValid(int x, int y, int width, int length);
std::vector<path_t> aStar(int grid[WIDTH][LENGTH], int width, int length, int startX, int startY, int endX, int endY, float biomeFactor, int neighbors);
std::vector<path_t> ConstructPath(int cameFrom[WIDTH * LENGTH], float gCost[WIDTH * LENGTH], int current, int start, int entity);
int PrintWeightMap(float map[WIDTH * LENGTH], int startX, int startY);

#endif