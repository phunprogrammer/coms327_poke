#include "PQueue.h"

#include "Config.h"

#ifndef ASTAR_H
#define ASTAR_H

typedef struct AStarNode {
    int x, y;
    float gCost, hCost, fCost;
    struct AStarNode *previous;
} asnode_t;

float as_calcDistCost(int currX, int currY, int endX, int endY);
asnode_t* as_createNode(int x, int y);
int as_isValid(int x, int y, int start, int width, int length);
path_t* aStar(int** grid, int width, int length, int startX, int startY, int endX, int endY, pqueue_t* open);
int InArray(int node, pqueue_t* queue);
path_t* ConstructPath(int cameFrom[WIDTH * LENGTH], int current, int start);

#endif