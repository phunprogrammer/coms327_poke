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

EntityTile* EntityManager::SpawnNPC(Entity entity) {
    NPCTile* npc = nullptr;

    switch (entity) {
        case HIKER:
            npc = new HikerTile(screen);
            break;
        case RIVAL:
            npc = new RivalTile(screen);
            break;
        case PACER:
            npc = new PacerTile(screen);
            break;
        case WANDERER:
            npc = new WandererTile(screen);
            break;
        case SENTRY:
            npc = new SentryTile(screen);
            break;
        case EXPLORER:
            npc = new ExplorerTile(screen);
            break;
        default:
            break;
    }
    npc->queueMove();

    return npc;
}