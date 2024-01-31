#include "PQueue.h"
#include "AStar.h"
#include "Config.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NEIGHBORS 4
#define NEIGHBORARR (vector_t[]) { { .x = 0, .y = -1 }, { .x = 1, .y = 0 }, { .x = 0, .y = 1 }, { .x = -1, .y = 0} }

float as_calcDistCost(int currX, int currY, int endX, int endY) {
    return abs(currX - endX) + abs(currY - endY);
}

asnode_t* as_createNode(int x, int y) {
    asnode_t* newNode;

    if (!(newNode = (asnode_t*)malloc(sizeof(asnode_t)))) return NULL;

    newNode->x = x;
    newNode->y = y;

    newNode->gCost = 0;
    newNode->hCost = 0;
    newNode->fCost = 0;
    newNode->value = 0;

    newNode->previous = NULL;
    
    return newNode;
}

int as_isValid(int x, int y, int start, int width, int length) {
    return (x >= start && x < length && y >= start && y < width);
}

asnode_t* aStar(float** grid, int width, int length, int startX, int startY, int endX, int endY) {
    printf("%f", grid[0][0]);
    asnode_t* startNode = as_createNode(startX, startY);
    asnode_t* endNode = as_createNode(endX, endY);

    pqueue_t open;
    pq_init(&open);

    pqueue_t closed;
    int index = 0;
    pq_init(&closed);

    pq_enqueue(&open, startNode, 0);

    while(!pq_isEmpty(&open)) {
        void *node;

        pq_dequeue(&open, &node);
        pq_enqueue(&closed, node, index++);

        asnode_t* currentNode = (asnode_t *)node;

        if(currentNode->x == endNode->x && currentNode->y == endNode->y) {
            pq_destroy(&open);
            pq_destroy(&closed);
            return currentNode;
        }

        for(int i = 0; i < NEIGHBORS; i++) {
            int nextX = currentNode->x + NEIGHBORARR[i].x;
            int nextY = currentNode->y + NEIGHBORARR[i].y;

            asnode_t* neighborNode = as_createNode(nextX, nextY);

            if(!as_isValid(nextX, nextY, 1, width - 1, length - 1) || InArray(nextX, nextY, &closed))
                continue;

            if(!InArray(nextX, nextY, &open)) {
                neighborNode->gCost = as_calcDistCost(nextX, nextY, startNode->x, startNode->y);
                neighborNode->hCost = as_calcDistCost(nextX, nextY, endNode->x, endNode->y);
                neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

                neighborNode->previous = currentNode;

                pq_enqueue(&open, neighborNode, neighborNode->fCost);
            }
        }
    }

    return NULL;
}

int InArray(int x, int y, pqueue_t* queue) {
    for(int i = 0; i < pq_size(queue); i++) {
        if((*(asnode_t *)(queue->array[i].data)).x == x && (*(asnode_t *)(queue->array[i].data)).y == y)
            return 1;
    }

    return 0;
}