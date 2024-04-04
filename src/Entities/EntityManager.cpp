#include "EntityManager.h"
#include "Screen.h"
#include <stdlib.h>

#include "AbstractTiles.h"
#include "HikerTile.h"
#include "RivalTile.h"
#include "PacerTile.h"
#include "WandererTile.h"
#include "ExplorerTile.h"
#include "SentryTile.h"

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
    if(numNPC <= 0)
        return 0;

    if(numNPC == 1) {
        SpawnNPC(ENTITIES[rand() % 2]);
        return 1;
    }

    SpawnNPC(Entity::RIVAL);
    SpawnNPC(Entity::HIKER);

    for(int i = 2; i < numNPC; i++) {
        SpawnNPC(ENTITIES[rand() % ENTITIES.size()]);
    }

    return 1;
}

int EntityManager::SpawnNPC(Entity entity) {
    NPCTile* npc;

    switch (entity) {
        case HIKER:
            npc = new HikerTile(screen);
            screen.getEntities().insert(npc->getCoord(), npc);
            break;
        case RIVAL:
            npc = new RivalTile(screen);
            screen.getEntities().insert(npc->getCoord(), npc);
            break;
        case PACER:
            npc = new PacerTile(screen);
            screen.getEntities().insert(npc->getCoord(), npc);
            break;
        case WANDERER:
            npc = new WandererTile(screen);
            screen.getEntities().insert(npc->getCoord(), npc);
            break;
        case SENTRY:
            npc = new SentryTile(screen);
            screen.getEntities().insert(npc->getCoord(), npc);
            break;
        case EXPLORER:
            npc = new ExplorerTile(screen);
            screen.getEntities().insert(npc->getCoord(), npc);
            break;
        default:
            return 0;
    }

    return 1;
}