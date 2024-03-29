#include "EntityGenerator.h"
#include "TerrainGenerator.h"
#include "EntityMover.h"
#include "aStar.h"
#include "PQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

int InitSize(screen_t* screen, int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--numtrainers") == 0 && i + 1 < argc) {
            numNPC = atoi(argv[i + 1]);
            break;
        }
    }
    if (!(screen->npcs = (entityType_t *)malloc(numNPC * sizeof(entityType_t)))) return 0;

    return 1;
}

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

int PlacePC(screen_t* screen, char cameFrom) {
    int x = 0;
    int y = 0;

    switch(cameFrom) {
        case 'w':
            y = screen->horizontalEndpoints.start;
            x = 0;
            break;
        case 'e':
            y = screen->horizontalEndpoints.end;
            x = LENGTH - 1;
            break;
        case 's':
            x = screen->verticalEndpoints.end;
            y = WIDTH - 1;
            break;
        case 'n':
            x = screen->verticalEndpoints.start;
            y = 0;
            break;
        default:
            return 0;
    }

    SetEntity(screen, &(screen->pc), x, y, PC);
    return 1;
}

int SpawnNPC(screen_t* screen, enum Tile entity) {
    int randX;
    int randY;

    do {
        randX = rand() % (LENGTH - 2) + 1;
        randY = rand() % (WIDTH - 2) + 1;
    } while(Entities[entity].weightFactor[screen->biomeMap[randY][randX].biomeID] == 0 || 
                screen->biomeMap[randY][randX].biomeID > BIOMENUM);

    if(entity == PACER || entity == WANDERER) {
        screen->npcs[screen->npcSize].direction.x = (rand() % 3) - 1;
        screen->npcs[screen->npcSize].direction.y = (screen->npcs[screen->npcSize].direction.x == 0) ? ((rand() % 2 == 0) ? -1 : 1) : (rand() % 3) - 1;
    }

    screen->npcs[screen->npcSize].defeated = 0;

    SetEntity(screen, &(screen->npcs[screen->npcSize++]), randX, randY, entity);

    return 0;
}

int SetEntity(screen_t* screen, entityType_t* entity, int x, int y, enum Tile entityID) {
    vector_t coord;
    coord.x = x;
    coord.y = y;

    path_t* tempPath = entity->entityPath;
    vector_t tempDir = entity->direction;
    int tempInt = entity->defeated;
    *entity = Entities[entityID];
    entity->coord = coord;
    entity->originalTile = screen->biomeMap[y][x];
    entity->entityPath = tempPath;
    entity->direction = tempDir;
    entity->defeated = tempInt;

    SwitchTile(&(screen->biomeMap[y][x]), Tiles[entityID]);
    AssignPathFunc(entity);

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

int SpawnAllNPC(screen_t* screen) {
    screen->npcSize = 0;
    
    if(numNPC <= 0)
        return 0;

    if(numNPC == 1) {
        SpawnNPC(screen, (enum Tile)(TILENUM - ENTITYNUM + (rand() % 2 + 1)));
        return 1;
    }

    SpawnNPC(screen, RIVAL);
    SpawnNPC(screen, HIKER);

    for(int i = 2; i < numNPC; i++) {
        SpawnNPC(screen, (enum Tile)(TILENUM - ENTITYNUM + (rand() % (ENTITYNUM - 1) + 1)));
    }

    return 1;
}