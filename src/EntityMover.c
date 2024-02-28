#include "Config.h"
#include "EntityMover.h"
#include "EntityGenerator.h"
#include "TerrainGenerator.h"
#include "PQueue.h"
#include "AStar.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int MoveEntity(screen_t* screen, entityType_t* entity, vector_t move) {
    if(move.x < 0 || move.x >= LENGTH || move.y < 0 || move.y >= WIDTH)
        return 0;

    if(fabs(entity->coord.x - move.x) > 1 || fabs(entity->coord.y - move.y) > 1)
        return 0;

    if(Entities[entity->tile.biomeID].weightFactor[screen->biomeMap[(int)move.y][(int)move.x].biomeID] == 0 ||
        screen->biomeMap[(int)move.y][(int)move.x].biomeID >= (TILENUM - ENTITYNUM))
        return 0;

    SwitchTile(&(screen->biomeMap[(int)entity->coord.y][(int)(int)entity->coord.x]), Tiles[entity->originalTile.biomeID]);
    SetEntity(screen, entity, move.x, move.y, entity->tile.biomeID);

    return 1;
}

int GetAllNPCMoves(screen_t* screen, pqueue_t* moveq) {
    pq_destroy_static(moveq);
    pq_init(moveq);

    float biomeFactor = 1.0;
    int neighbors = 8;

    int biomeGrid[WIDTH][LENGTH];

    for (int i = 0; i < screen->npcSize; i++) {
        for (int y = 0; y < WIDTH; y++) {
            for (int x = 0; x < LENGTH; x++) {
                biomeGrid[y][x] = screen->npcs[i].weightFactor[screen->biomeMap[y][x].biomeID];
            }
        }

        biomeGrid[(int)screen->npcs[i].coord.y][(int)screen->npcs[i].coord.x] = screen->npcs[i].weightFactor[(int)screen->npcs[i].originalTile.biomeID];
        path_t* entityPath = aStar(biomeGrid, WIDTH - 2, LENGTH - 2, screen->pc.coord.x, screen->pc.coord.y, screen->npcs[i].coord.x, screen->npcs[i].coord.y, biomeFactor, neighbors);
        screen->npcs[i].entityPath = entityPath;

        while(entityPath->previous != NULL) {
            pq_enqueue(moveq, &(screen->npcs[i]), entityPath->gCost);
            entityPath = entityPath->previous;
        }
        
        void* data;
        pq_dequeue(moveq, &data);
        entityType_t* npc = (entityType_t*)data;
        npc->entityPath = npc->entityPath->previous;
    }

    return 1;
}

int PCController(screen_t* screen, char input) {
    vector_t move;

    switch(input) {
        case 'w':
            move.x = screen->pc.coord.x;
            move.y = screen->pc.coord.y - 1;
            break;
        case 'a':
            move.x = screen->pc.coord.x - 1;
            move.y = screen->pc.coord.y;
            break;
        case 's':
            move.x = screen->pc.coord.x;
            move.y = screen->pc.coord.y + 1;
            break;
        case 'd':
            move.x = screen->pc.coord.x + 1;
            move.y = screen->pc.coord.y;
            break;
        default:
            return 0;
    }

    MoveEntity(screen, &(screen->pc), move);
    return 1;
}