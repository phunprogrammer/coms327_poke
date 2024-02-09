#include "Config.h"
#include "EntityGenerator.h"
#include "TerrainGenerator.h"
#include <stdlib.h>
#include <stdio.h>

int RandomizePC(screen_t* screen) {
    srand(FirstFourDigits(screen->biomeMap[0][0].minHeight));
    path_t* horizontal = screen->horizontalPath;
    path_t* vertical = screen->verticalPath;

    int randPathLoc = rand() % (LENGTH + WIDTH) * 2;
    printf("%d\n", randPathLoc);

    int i = 0;

    while (horizontal != NULL) {
        if(i++ == randPathLoc) {
            SetEntity(screen, horizontal->coord.x, horizontal->coord.y, PC);
            return 0;
        }
        horizontal = horizontal->previous;
    }

    while (vertical != NULL) {
        if(i++ == randPathLoc) {
            SetEntity(screen, vertical->coord.x, vertical->coord.y, PC);
            return 0;
        }
        vertical = vertical->previous;
    }

    int randX = rand() % (LENGTH - 2) + 1;
    int randY = rand() % (WIDTH - 2) + 1;

    SetEntity(screen, randX, randY, PC);

    return 0;
}

entityType_t SetEntity(screen_t* screen, int x, int y, enum Tile entity) {
    entityType_t currEntity = { .tile = Tiles[entity], .originalTile = screen->biomeMap[y][x].biomeID };
    SwitchTile(&(screen->biomeMap[y][x]), Tiles[entity]);

    screen->entities[screen->entitiesSize] = currEntity;
    return screen->entities[screen->entitiesSize++];
}