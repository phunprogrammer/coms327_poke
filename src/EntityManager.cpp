#include "EntityManager.h"
#include "Screen.h"
#include "Entities.h"
#include <stdlib.h>

EntityManager::EntityManager(Screen& screen) : screen(screen) {}


int EntityManager::RandomizePC() {
    int randPathLoc = rand() % (LENGTH + WIDTH) * 2 ;

    int i = 0;

    for(; i < screen.getPaths().horizontalPath.size(); i++) {
        coord_t current = screen.getPaths().horizontalPath.at(i).coord;
        if(i == randPathLoc) {
            PCTile* pc = new PCTile(screen, current);
            pc->Spawn();
            return 0;
        }
    }

    for(; i < screen.getPaths().verticalPath.size(); i++) {
        coord_t current = screen.getPaths().verticalPath.at(i).coord;
        
        if(i == randPathLoc) {
            PCTile* pc = new PCTile(screen, current);
            pc->Spawn();
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
    PCTile* pc = new PCTile(screen, random);
    pc->Spawn();

    return 0;
}

int EntityManager::PlacePC(char cameFrom) {

}

int EntityManager::SpawnNPC(Entity entity) {

}

int EntityManager::SpawnAllNPC() {

}