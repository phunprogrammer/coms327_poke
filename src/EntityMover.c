#include "Config.h"
#include "EntityMover.h"
#include "EntityGenerator.h"
#include "TerrainGenerator.h"
#include <math.h>

int MovePC(screen_t* screen, vector_t move) {
    if(move.x < 0 || move.x >= LENGTH || move.y < 0 || move.y >= WIDTH)
        return 0;

    if(fabs(screen->pc.coord.x - move.x) > 1 || fabs(screen->pc.coord.y - move.y) > 1)
        return 0;

    if(Entities[PC].weightFactor[screen->biomeMap[(int)move.y][(int)move.x].biomeID] == 0 ||
        Entities[PC].weightFactor[screen->biomeMap[(int)move.y][(int)move.x].biomeID] > 100)
        return 0;

    SwitchTile(&(screen->biomeMap[(int)screen->pc.coord.y][(int)(int)screen->pc.coord.x]), Tiles[screen->pc.originalTile]);
    SetEntity(screen, &(screen->pc), move.x, move.y, PC);

    return 1;
}

// int GetAllNPCMoves(screen_t* screen, pqueue_t* moveq) {
//     float biomeFactor = 1.0;
//     int neighbors = 8;

//     int biomeGrid[WIDTH][LENGTH];

//     for (int i = 0; i < screen->npcSize; i++) {
//         for (int y = 0; y < WIDTH; y++) {
//             for (int x = 0; x < LENGTH; x++) {
//                 biomeGrid[y][x] = npcs[i].weightFactor[screen->biomeMap[y][x].biomeID];
//             }
//         }

//         path_t* entityPath = aStar(biomeGrid, WIDTH - 2, LENGTH - 2, screen->pc.coord.x, screen->pc.coord.y, npcs[i].coord.x, npcs[i].coord.y, biomeFactor, neighbors);

//         while(entityPath != NULL) {
//             pq_enqueue(moveq, entityPath.)

//             entityPath = entityPath->previous;
//         }
//     }
// }

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

    MovePC(screen, move);
    return 1;
}