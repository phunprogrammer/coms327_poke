#include "EntityGenerator.h"
#include "TerrainGenerator.h"
#include "aStar.h"
#include "PQueue.h"
#include <stdlib.h>
#include <stdio.h>

int RandomizePC(screen_t* screen) {
    srand(FirstFourDigits(screen->biomeMap[0][0].minHeight));
    path_t* horizontal = screen->horizontalPath;
    path_t* vertical = screen->verticalPath;

    int randPathLoc = rand() % (LENGTH + WIDTH) * 2 ;

    int i = 0;

    while (horizontal != NULL) {
        if(i++ == randPathLoc) {
            SetEntity(screen, &(screen->pc), horizontal->coord.x, horizontal->coord.y, PC);
            return 0;
        }
        horizontal = horizontal->previous;
    }

    while (vertical != NULL) {
        if(i++ == randPathLoc) {
            SetEntity(screen, &(screen->pc), vertical->coord.x, vertical->coord.y, PC);
            return 0;
        }
        vertical = vertical->previous;
    }

    int randX;
    int randY;

    do {
        randX = rand() % (LENGTH - 2) + 1;
        randY = rand() % (WIDTH - 2) + 1;
    } while(Entities[PC].weightFactor[screen->biomeMap[randY][randX].biomeID] == 0);

    SetEntity(screen, &(screen->pc), randX, randY, PC);

    return 0;
}

int SpawnNPC(screen_t* screen, enum Tile entity) {
    int randX;
    int randY;

    do {
        randX = rand() % (LENGTH - 2) + 1;
        randY = rand() % (WIDTH - 2) + 1;
    } while(Entities[entity].weightFactor[screen->biomeMap[randY][randX].biomeID] == 0 || 
                screen->biomeMap[randY][randX].biomeID > BIOMENUM);

    SetEntity(screen, &(screen->npcs[screen->npcSize++]), randX, randY, entity);
    return 0;
}

int SetEntity(screen_t* screen, entityType_t* entity, int x, int y, enum Tile entityID) {
    vector_t coord = { .x = x, .y = y };

    path_t* tempPath = entity->entityPath;
    *entity = Entities[entityID];
    entity->coord = coord;
    entity->originalTile = screen->biomeMap[y][x];
    entity->entityPath = tempPath;

    SwitchTile(&(screen->biomeMap[y][x]), Tiles[entityID]);

    return 0;
}

int GenWeightMap(screen_t* screen, entityType_t entity) {
    int endX = 0;
    int endY = 0;
    float biomeFactor = 1.0;
    int neighbors = 8;

    int biomeGrid[WIDTH][LENGTH];

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = entity.weightFactor[screen->biomeMap[y][x].biomeID];
        }
    }

    aStar(biomeGrid, WIDTH - 2, LENGTH - 2, screen->pc.coord.x, screen->pc.coord.y, endX, endY, biomeFactor, neighbors);

    return 0;
}