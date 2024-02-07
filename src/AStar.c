#include "PQueue.h"
#include "AStar.h"
#include "Config.h"
#include <stdlib.h>
#include <math.h>

#define NEIGHBORS 4
#define BIOMEFACTOR 0.08
#define NEIGHBORARR (vector_t[]) { { .x = 0, .y = -1 }, { .x = -1, .y = 0 }, { .x = 1, .y = 0 }, { .x = 0, .y = 1 }, { .x = -1, .y = -1 }, { .x = 1, .y = -1 }, { .x = -1, .y = 1 }, { .x = 1, .y = 1 } }

float as_calcDistCost(int currX, int currY, int endX, int endY) {
    return sqrt(pow(abs(currX - endX), 2) + pow(abs(currY - endY), 2));
}

asnode_t* as_createNode(int x, int y) {
    asnode_t* newNode;

    if (!(newNode = (asnode_t*)malloc(sizeof(asnode_t)))) return NULL;

    newNode->x = x;
    newNode->y = y;

    newNode->previous = NULL;
    
    return newNode;
}

int as_isValid(int x, int y, int start, int width, int length) {
    return (x >= start && x < length && y >= start && y < width);
}

path_t* aStar(int** grid, int width, int length, int startX, int startY, int endX, int endY, pqueue_t* open) {
    int startKey = startY * LENGTH + startX;
    int endKey = endY * LENGTH + endX;

    pq_init(open);

    int closed[WIDTH * LENGTH] = { 0 };

    int cameFrom[WIDTH * LENGTH] = { 0 };

    float gCost[WIDTH * LENGTH] = { 0 };
    gCost[startKey] = 1;

    float fCost[WIDTH * LENGTH] = { 0 };
    fCost[startKey] = as_calcDistCost(startX, startY, endX, endY) * (float)Tiles[CLEARING].weight * BIOMEFACTOR;

    pq_enqueue(open, &startKey, fCost[startKey]);

    while(!pq_isEmpty(open)) {
        void *node;

        pq_dequeue(open, &node);
        int currentKey = *(int *)node;
        closed[currentKey] = 1;

        if(currentKey == endKey) {
            pq_destroy_dynamic(open);
            return ConstructPath(cameFrom, currentKey, startKey);
        }

        for(int i = 0; i < NEIGHBORS; i++) {
            int nextX = currentKey % LENGTH + NEIGHBORARR[i].x;
            int nextY = currentKey / LENGTH + NEIGHBORARR[i].y;

            int* neighborNode = (int *)malloc(sizeof(int));
            *neighborNode = nextY * LENGTH + nextX;

            if(!as_isValid(nextX, nextY, 2, width - 2, length - 2) || closed[*neighborNode] == 1) {
                free(neighborNode);
                continue;
            }
            
            int visited = gCost[*neighborNode] != 0;
            int costToNeighbor = gCost[currentKey] + as_calcDistCost(cameFrom[currentKey] % LENGTH, cameFrom[currentKey] / LENGTH, nextX, nextY) / 2 + (float)grid[nextY][nextX] * BIOMEFACTOR;

            if(costToNeighbor < gCost[*neighborNode] || !visited) {
                cameFrom[*neighborNode] = currentKey;
                gCost[*neighborNode] = costToNeighbor;
                fCost[*neighborNode] = costToNeighbor + as_calcDistCost(nextX, nextY, endX, endY) * (float)Tiles[CLEARING].weight * BIOMEFACTOR;

                if(!visited)
                    pq_enqueue(open, neighborNode, fCost[*neighborNode]);
            }
            else
                free(neighborNode);
        }
    }

    return NULL;
}   

path_t* ConstructPath(int cameFrom[WIDTH * LENGTH], int current, int start) {
        path_t* path = (path_t *)malloc(sizeof(path_t));
        path->coord.x = current % LENGTH;
        path->coord.y = current / LENGTH;
        path->previous = NULL;
        path_t* tempPath = path;

        while (current != start) {
            current = cameFrom[current];
            
            tempPath->previous = (path_t *)malloc(sizeof(path_t));
            tempPath->previous->coord.x = current % LENGTH;
            tempPath->previous->coord.y = current / LENGTH;
            tempPath->previous->previous = NULL;
            
            tempPath = tempPath->previous;
        }

        return path;
}

int InArray(int node, pqueue_t* queue) {
    for(int i = 0; i < pq_size(queue); i++) {
        int currentNode = *((int*)(queue->array[i].data));

        if(currentNode == node) {
            return 1;
        }
    }

    return 0;
}