#include "EntityManager.h"
#include "Screen.h"
#include <stdlib.h>

#include "AbstractTiles.h"

EntityManager::EntityManager(Screen& screen) : screen(screen) {}

int EntityManager::RandomizePC() {
    int randPathLoc = rand() % (LENGTH + WIDTH) * 2 ;

    int i = 0;

    for(; i < (int)screen.getPaths().horizontalPath.size(); i++) {
        coord_t current = screen.getPaths().horizontalPath.at(i).coord;
        if(i == randPathLoc) {
            screen.getEntities()[0]->setCoord(current);
            return 0;
        }
    }

    for(; i < (int)screen.getPaths().verticalPath.size(); i++) {
        coord_t current = screen.getPaths().verticalPath.at(i).coord;
        
        if(i == randPathLoc) {
            screen.getEntities()[0]->setCoord(current);
            return 0;
        }
    }

    int randX;
    int randY;

    do {
        randX = rand() % (LENGTH - 2) + 1;
        randY = rand() % (WIDTH - 2) + 1;
    } while(PC_SPEED.at(screen.getTerrainMap()[randY][randX].getTerrain()) == 0);

    coord_t random = { randX, randY };
    screen.getEntities()[0]->setCoord(random);

    return 0;
}

int EntityManager::SpawnAllNPC() {

}