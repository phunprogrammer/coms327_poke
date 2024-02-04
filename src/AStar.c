#include "PQueue.h"
#include "AStar.h"
#include "Config.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NEIGHBORS 4
#define NEIGHBORARR (vector_t[]) { { .x = 0, .y = -1 }, { .x = 1, .y = 0 }, { .x = 0, .y = 1 }, { .x = -1, .y = 0} }

float as_calcDistCost(float currX, float currY, float endX, float endY) {
    return (float)sqrt(pow(fabs(currX - endX), 2) + pow(fabs(currY - endY), 2));
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

            void* tempNode = 0;

            if(!as_isValid(nextX, nextY, 2, width - 2, length - 2) || InArray(nextX, nextY, &closed, &tempNode))
                continue;

            int inArray = InArray(nextX, nextY, &open, &tempNode);
            int costToNeighbor = currentNode->gCost + as_calcDistCost(currentNode->x, currentNode->y, nextX, nextY);

            if((inArray && costToNeighbor < ((asnode_t *)tempNode)->gCost) || !inArray) {
                neighborNode->gCost = costToNeighbor;
                neighborNode->hCost = as_calcDistCost(nextX, nextY, endNode->x, endNode->y);
                neighborNode->fCost = neighborNode->gCost + neighborNode->hCost + grid[nextY][nextX];;
                neighborNode->previous = currentNode;

                pq_enqueue(&open, neighborNode, neighborNode->fCost);
            }
        }
    }

    return NULL;
}

int InArray(int x, int y, pqueue_t* queue, void** data) {
    for(int i = 0; i < pq_size(queue); i++) {
        if((*(asnode_t *)(queue->array[i].data)).x == x && (*(asnode_t *)(queue->array[i].data)).y == y) {
            *data = (asnode_t *)(queue->array[i].data);
            return 1;
        }
    }

    return 0;
}