#include "PQueue.h"
#include "AStar.h"
#include "Config.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NEIGHBORARR (vector_t[]) { { .x = 0, .y = -1 }, { .x = -1, .y = 0 }, { .x = 1, .y = 0 }, { .x = 0, .y = 1 }, { .x = -1, .y = -1 }, { .x = 1, .y = -1 }, { .x = -1, .y = 1 }, { .x = 1, .y = 1 } }

/**
 * @brief Calculates the distance cost between two points
 * 
 * @param currX pointA x
 * @param currY pointA y
 * @param endX pointB x
 * @param endY pointB y
 * @return float 
 */
float as_calcDistCost(int currX, int currY, int endX, int endY) {
    return sqrt(pow(abs(currX - endX), 2) + pow(abs(currY - endY), 2));
}

/**
 * @brief Detects if the point given is a valid point based on given length and width
 * 
 * @param x 
 * @param y 
 * @param start 
 * @param width 
 * @param length 
 * @return int 0 or 1
 */
int as_isValid(int x, int y, int width, int length) {
    int widthOffset = (WIDTH - width) / 2;
    int lengthOffest = (LENGTH - length) / 2;

    return (x >= lengthOffest && x < length + lengthOffest && y >= widthOffset && y < width + widthOffset);
}

/**
 * @brief A* path finding algorithm finds the optimal path based on the given map grid
 * 
 * @param grid A grid of weights
 * @param width Width of the grid
 * @param length length of the grid
 * @param startX Starting position's X value
 * @param startY Starting position's Y value
 * @param endX Ending position's X value
 * @param endY Ending position's Y value
 * @param open An open queue that stores opened grids
 * @return path_t* A pointer from the end of the path to the beginning
 */
path_t* aStar(int grid[WIDTH][LENGTH], int width, int length, int startX, int startY, int endX, int endY, float biomeFactor, int neighbors, pqueue_t* open) {
    int startKey = startY * LENGTH + startX;
    int endKey = endY * LENGTH + endX;

    pq_init(open);

    int closed[WIDTH * LENGTH] = { 0 };

    int cameFrom[WIDTH * LENGTH] = { 0 };

    float gCost[WIDTH * LENGTH] = { 0 };
    gCost[startKey] = 0;

    float fCost[WIDTH * LENGTH] = { 0 };
    fCost[startKey] = as_calcDistCost(startX, startY, endX, endY) * (float)Tiles[CLEARING].weight * biomeFactor;

    pq_enqueue(open, &startKey, fCost[startKey]);

    while(!pq_isEmpty(open)) {
        void *node;

        pq_dequeue(open, &node);
        int currentKey = *(int *)node;
        closed[currentKey] = 1;

        if(currentKey == endKey) {
            pq_destroy(open);
            return ConstructPath(cameFrom, currentKey, startKey);
        }

        for(int i = 0; i < neighbors; i++) {
            int nextX = currentKey % LENGTH + NEIGHBORARR[i].x;
            int nextY = currentKey / LENGTH + NEIGHBORARR[i].y;

            int neighborNode = nextY * LENGTH + nextX;

            if(!as_isValid(nextX, nextY, width, length) || grid[nextY][nextX] == 0 || closed[neighborNode] == 1) {
                continue;
            }
            
            int visited = gCost[neighborNode] != 0;
            int costToNeighbor = gCost[currentKey] + as_calcDistCost(cameFrom[currentKey] % LENGTH, cameFrom[currentKey] / LENGTH, nextX, nextY) / 2 * !(int)biomeFactor + (float)grid[nextY][nextX] * biomeFactor;

            if(costToNeighbor < gCost[neighborNode] || !visited) {
                cameFrom[neighborNode] = currentKey;
                gCost[neighborNode] = costToNeighbor;
                fCost[neighborNode] = costToNeighbor + as_calcDistCost(nextX, nextY, endX, endY) * (float)Tiles[CLEARING].weight * biomeFactor;

                if(!visited) {
                    int* neighborPtr = (int *)malloc(sizeof(int));
                    *neighborPtr = neighborNode;
                    pq_enqueue(open, neighborPtr, fCost[neighborNode]);
                    continue;
                }
            }
        }
    }

    PrintWeightMap(gCost, startX, startY);
    pq_destroy(open);
    return NULL;
}   

/**
 * @brief Reconstructs the path from the cameFrom array and the current position
 * 
 * @param cameFrom Latest traveled path array
 * @param current Starting point of the path
 * @param start Stopping point of the path
 * @return path_t* 
 */
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

int PrintWeightMap(float map[WIDTH * LENGTH], int startX, int startY) {
    for(int i = 0; i < WIDTH; i++) {
        for(int j = 0; j < LENGTH; j++) {
            int current = (int)map[i * LENGTH + j];

            if(current == 0 && !(i == startY && j == startX)) {
                printf("   ");
                continue;
            }
                
            printf("%02d ", current % 100);
        }
        printf("\n");
    }

    return 0;
}