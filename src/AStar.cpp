#include "PQItem.h"
#include "AStar.h"
#include "Config.h"
#include "Tiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <queue>
#include <iostream>

const coord_t NEIGHBORARR[] = {
    {0, -1},
    {-1, 0}, 
    {1, 0}, 
    {0, 1},  
    {-1, -1}, 
    {1, -1},
    {-1, 1}, 
    {1, 1} 
};

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
std::vector<path_t> aStar(int grid[WIDTH][LENGTH], int width, int length, int startX, int startY, int endX, int endY, float biomeFactor, int neighbors) {
    int startKey = startY * LENGTH + startX;
    int endKey = endY * LENGTH + endX;

    grid[endKey / LENGTH][endKey % LENGTH] = -1;

    std::priority_queue<PQItem<int>> pq;

    int closed[WIDTH * LENGTH] = { 0 };

    int cameFrom[WIDTH * LENGTH] = { 0 };

    float gCost[WIDTH * LENGTH] = { 0 };
    gCost[startKey] = 0;

    float fCost[WIDTH * LENGTH] = { 0 };
    fCost[startKey] = as_calcDistCost(startX, startY, endX, endY) * (float)WEIGHTS.at(Terrain::CLEARING) * biomeFactor;

    pq.push(PQItem(startKey, fCost[startKey]));

    while(!pq.empty()) {
        int currentKey = pq.top().getData();
        pq.pop();

        closed[currentKey] = 1;

        if(currentKey == endKey) {
            return ConstructPath(cameFrom, gCost, currentKey, startKey, neighbors == 8);
        }

        for(int i = 0; i < neighbors; i++) {
            int nextX = currentKey % LENGTH + NEIGHBORARR[i].x;
            int nextY = currentKey / LENGTH + NEIGHBORARR[i].y;

            int neighborNode = nextY * LENGTH + nextX;

            if(!as_isValid(nextX, nextY, width, length) || grid[nextY][nextX] == 0 || closed[neighborNode] == 1) {
                continue;
            }
            
            int visited = gCost[neighborNode] != 0;
            int costToNeighbor = gCost[currentKey] + as_calcDistCost(cameFrom[currentKey] % LENGTH, cameFrom[currentKey] / LENGTH, nextX, nextY) / 2 * (neighbors != 8) + (float)grid[nextY][nextX] * biomeFactor;

            if(costToNeighbor < gCost[neighborNode] || !visited) {
                cameFrom[neighborNode] = currentKey;
                gCost[neighborNode] = costToNeighbor;
                fCost[neighborNode] = costToNeighbor + as_calcDistCost(nextX, nextY, endX, endY) * (float)WEIGHTS.at(Terrain::CLEARING) * biomeFactor;

                if(!visited) {
                    pq.push(PQItem(neighborNode, fCost[neighborNode]));
                    continue;
                }
            }
        }
    }

    //PrintWeightMap(gCost, startX, startY);
    std::vector<path_t> paths;
    return paths;
}   

/**
 * @brief Reconstructs the path from the cameFrom array and the current position
 * 
 * @param cameFrom Latest traveled path array
 * @param current Starting point of the path
 * @param start Stopping point of the path
 * @return path_t* 
 */
std::vector<path_t> ConstructPath(int cameFrom[WIDTH * LENGTH], float gCost[WIDTH * LENGTH], int current, int start, int entity) {
        std::vector<path_t> paths;
        path_t path;
        path.coord.x = current % LENGTH;
        path.coord.y = current / LENGTH;
        path.gCost = (int)gCost[current];
        paths.push_back(path);
        
        while (current != start) {
            if(entity && cameFrom[current] == start) {
                //path = path->previous;
                break;
            }
            
            current = cameFrom[current];
            
            path_t path;
            path.coord.x = current % LENGTH;
            path.coord.y = current / LENGTH;
            path.gCost = (int)gCost[current];
            paths.push_back(path);
        }

        return paths;
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