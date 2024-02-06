#include "PQueue.h"

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
asnode_t* aStar(int** grid, int width, int length, int startX, int startY, int endX, int endY, int previousX, int previousY, pqueue_t* open, pqueue_t* closed);
int InArray(int x, int y, pqueue_t* queue, int* index);

#endif