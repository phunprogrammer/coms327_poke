#include "PQueue.h"

#ifndef ASTAR_H
#define ASTAR_H

typedef struct AStarNode {
    int x, y;
    float gCost, hCost, fCost, steps;
    struct AStarNode *previous;
} asnode_t;

float as_calcDistCost(float currX, float currY, float endX, float endY);
asnode_t* as_createNode(int x, int y);
int as_isValid(int x, int y, int start, int width, int length);
asnode_t* aStar(float** grid, int width, int length, int startX, int startY, int endX, int endY, int previousX, int previousY);
int InArray(int x, int y, pqueue_t* queue, int* index);

#endif