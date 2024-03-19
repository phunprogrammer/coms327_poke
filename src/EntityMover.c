#include "Config.h"
#include "EntityMover.h"
#include "EntityGenerator.h"
#include "TerrainGenerator.h"
#include "PQueue.h"
#include "AStar.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <ncurses.h>

int ValidMove(screen_t* screen, entityType_t* entity, vector_t move) {
    if(move.x < 0 || move.x >= LENGTH || move.y < 0 || move.y >= WIDTH)
        return 0;

    if(fabs(entity->coord.x - move.x) > 1 || fabs(entity->coord.y - move.y) > 1)
        return 0;

    if(Entities[entity->tile.biomeID].weightFactor[screen->biomeMap[(int)move.y][(int)move.x].biomeID] == 0 ||
        screen->biomeMap[(int)move.y][(int)move.x].biomeID >= (TILENUM - ENTITYNUM))
        return 0;

    return 1;
}

int MoveEntity(screen_t* screen, entityType_t* entity, vector_t move) {
    if(!ValidMove(screen, entity, move))
        return 0;

    SwitchTile(&(screen->biomeMap[(int)entity->coord.y][(int)(int)entity->coord.x]), Tiles[entity->originalTile.biomeID]);
    SetEntity(screen, entity, move.x, move.y, entity->tile.biomeID);

    return 1;
}

int GetAllNPCMoves(screen_t* screen, pqueue_t* moveq, int currentPriority) {
    for (int i = 0; i < screen->npcSize; i++) {
        AddPathToQ(moveq, screen, i, currentPriority);
    }

    return 1;
}

int AddPathToQ(pqueue_t* moveq, screen_t* screen, int entityIndex, int currentPriority) {
    path_t* entityPath = screen->npcs[entityIndex].getPath(screen, &screen->npcs[entityIndex]);

    // path_t* printPath = entityPath;
    // int line = 20;
    // while(printPath != NULL) {
    //     mvprintw(line++, 0,"%d: (%d, %d), Score: %d", entityIndex, (int)printPath->coord.x, (int)printPath->coord.y, printPath->gCost + currentPriority);
    //     printPath = printPath->previous;
    // }

    if(entityPath == NULL) return 0;

    while(entityPath->previous != NULL) {
        path_t* tempPath = entityPath;
        entityPath = entityPath->previous;
        free(tempPath);
    } 

    entityMove_t* move = (entityMove_t*)malloc(sizeof(entityMove_t));

    move->entityIndex = entityIndex;
    move->coord.x = entityPath->coord.x;
    move->coord.y = entityPath->coord.y;
    move->priority = entityPath->gCost + currentPriority;

    pq_enqueue(moveq, move, entityPath->gCost + currentPriority);
    free(entityPath);

    return 1;
}

int AssignPathFunc(entityType_t* entity) {
    switch(entity->tile.biomeID) {
        case HIKER:
            entity->getPath = GetHikerPath;
            break;
        case RIVAL:
            entity->getPath = GetRivalPath;
            break;
        case PACER:
            entity->getPath = GetPacerPath;
            break;
        case WANDERER:
            entity->getPath = GetWandererPath;
            break;
        case EXPLORER:
            entity->getPath = GetExplorerPath;
            break;
        default:
            return 0;
    }

    return 1;
}

path_t* GetHikerPath (screen_t* screen, entityType_t* entity) {
    float biomeFactor = 1.0;
    int neighbors = 8;
    int biomeGrid[WIDTH][LENGTH];

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = entity->weightFactor[screen->biomeMap[y][x].biomeID];
        }
    }

    biomeGrid[(int)entity->coord.y][(int)entity->coord.x] = entity->weightFactor[(int)entity->originalTile.biomeID];
    return aStar(biomeGrid, WIDTH - 2, LENGTH - 2, entity->coord.x, entity->coord.y, screen->pc.coord.x, screen->pc.coord.y, biomeFactor, neighbors);
}

path_t* GetRivalPath (screen_t* screen, entityType_t* entity) {
    float biomeFactor = 1.0;
    int neighbors = 8;
    int biomeGrid[WIDTH][LENGTH];

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = entity->weightFactor[screen->biomeMap[y][x].biomeID];
        }
    }

    biomeGrid[(int)entity->coord.y][(int)entity->coord.x] = entity->weightFactor[(int)entity->originalTile.biomeID];
    return aStar(biomeGrid, WIDTH - 2, LENGTH - 2, entity->coord.x, entity->coord.y, screen->pc.coord.x, screen->pc.coord.y, biomeFactor, neighbors);
}

path_t* GetPacerPath (screen_t* screen, entityType_t* entity) {
    vector_t move = { .x = entity->direction.x + entity->coord.x, .y = entity->direction.y + entity->coord.y };

    if(!ValidMove(screen, entity, move)) {
        entity->direction.x *= -1;
        entity->direction.y *= -1;
    }

    enum Tile nextTile = screen->biomeMap[(int)(entity->coord.y + entity->direction.y)][(int)(entity->coord.x + entity->direction.x)].biomeID;

    path_t* path = (path_t *)malloc(sizeof(path_t));
    path->coord.x = entity->coord.x + entity->direction.x;
    path->coord.y = entity->coord.y + entity->direction.y;
    path->gCost = entity->weightFactor[nextTile];
    path->previous = NULL;

    return path;
}

path_t* GetWandererPath (screen_t* screen, entityType_t* entity) {
    int direction = rand() % 4;
    int directionX[4] = { 0, 1, 0, -1 };
    int directionY[4] = { -1, 0, 1, 0 };

    path_t* path = (path_t *)malloc(sizeof(path_t));
    path->coord.x = entity->coord.x;
    path->coord.y = entity->coord.y;
    path->gCost = 0;
    path->previous = NULL;
    path_t* tempPath = path;
    enum Tile originalTile = entity->originalTile.biomeID;
    do {    
        //printf("(%f, %f)\n", tempPath->coord.x + directionX, tempPath->coord.y + directionY);      
        tempPath->previous = (path_t *)malloc(sizeof(path_t));
        tempPath->previous->coord.x = tempPath->coord.x + directionX[direction];
        tempPath->previous->coord.y = tempPath->coord.y + directionY[direction];
        tempPath->previous->gCost = tempPath->gCost + entity->weightFactor[screen->biomeMap[(int)tempPath->coord.y + directionY[direction]][(int)tempPath->coord.x + directionX[direction]].biomeID];
        tempPath->previous->previous = NULL;
        
        tempPath = tempPath->previous;
    } while(entity->weightFactor[screen->biomeMap[(int)tempPath->coord.y][(int)tempPath->coord.x].biomeID] != 0 || originalTile == screen->biomeMap[(int)tempPath->coord.y][(int)tempPath->coord.x].biomeID);

    return path;
}

path_t* GetExplorerPath (screen_t* screen, entityType_t* entity) {
    int direction = rand() % 4;
    int directionX[4] = { 0, 1, 0, -1 };
    int directionY[4] = { -1, 0, 1, 0 };

    path_t* path = (path_t *)malloc(sizeof(path_t));
    path->coord.x = entity->coord.x;
    path->coord.y = entity->coord.y;
    path->gCost = 0;
    path->previous = NULL;
    path_t* tempPath = path;

    while(entity->weightFactor[screen->biomeMap[(int)tempPath->coord.y][(int)tempPath->coord.x].biomeID] != 0) {    
        //printf("(%f, %f)\n", tempPath->coord.x + directionX, tempPath->coord.y + directionY);      
        tempPath->previous = (path_t *)malloc(sizeof(path_t));
        tempPath->previous->coord.x = tempPath->coord.x + directionX[direction];
        tempPath->previous->coord.y = tempPath->coord.y + directionY[direction];
        tempPath->previous->gCost = tempPath->gCost + entity->weightFactor[screen->biomeMap[(int)tempPath->coord.y + directionY[direction]][(int)tempPath->coord.x + directionX[direction]].biomeID];
        tempPath->previous->previous = NULL;
        
        tempPath = tempPath->previous;
    }

    return path;
}