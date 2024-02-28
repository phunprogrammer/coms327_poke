#include "NoiseGenerator.h"
#include "PPMGenerator.h"
#include "TerrainGenerator.h"
#include "Config.h"
#include "PQueue.h"
#include "EntityGenerator.h"
#include "EntityMover.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void DevLoop(screen_t* screen, waves_t waves, int currX, int currY) {
    char input = 0;
    do {
        switch (input) {
            case 'n':
                UpdateOffset(currX, (currY = fmax(--currY, MINSIZE)));
                break;
            case 'e':
                UpdateOffset((currX = fmin(++currX, MAXSIZE)), currY);
                break;
            case 's':
                UpdateOffset(currX, (currY = fmin(++currY, MAXSIZE)));
                break;
            case 'w':
                UpdateOffset((currX = fmax(--currX, MINSIZE)), currY);
                break;
            case 'f':
                int x, y;
                scanf("%d %d", &x, &y);
                UpdateOffset((currX = (int)fmax(fmin(MIDDLEX + x, MAXSIZE), MINSIZE)), (currY = (int)fmax(fmin(MIDDLEY + y, MAXSIZE), MINSIZE)));
                break;
            case 0:
                break;
            case '\n':
                continue;
            default:
                printf("ERROR: Not a valid command\n");
                continue;
        }
        
        *screen = ScreenGenerator(waves);
        RandomizePC(screen);
        SpawnNPC(screen, HIKER);

        for(int y = 0; y < WIDTH; y++) {
            for(int x = 0; x < LENGTH; x++) {
                printf("%c", screen->biomeMap[y][x].type);
            }
            printf("\n");
        } 
        printf("(%d, %d)\n", currX - MIDDLEX, currY - MIDDLEY);

        DestroyScreen(screen);

    } while ((input = getc(stdin)) != 'q');
}

void GameLoop(screen_t* screen, waves_t waves, int currX, int currY) {
    fd_set detectInput;
    struct timeval timeout;
    char input[1024];

    *screen = ScreenGenerator(waves);
    RandomizePC(screen);

    pqueue_t moveQueue;
    pq_init(&moveQueue);

    GetAllNPCMoves(screen, &moveQueue);

    int currentPriority = 0;

    while (1) {        
        for(int y = 0; y < WIDTH; y++) {
            for(int x = 0; x < LENGTH; x++) {
                printf("%c", screen->biomeMap[y][x].type);
            }
            printf("\n");
        } 

        printf("(%d, %d)\n", currX - MIDDLEX, currY - MIDDLEY);

        FD_ZERO(&detectInput);
        FD_SET(0, &detectInput);
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        currentPriority = moveQueue.array[pq_size(&moveQueue) - 1].priority;

        if(select(1, &detectInput, NULL, NULL, &timeout) == 1) {
            char buffer[1024];
            fgets(buffer, sizeof(buffer), stdin);
            sprintf(input, "%s", buffer);

            if (input[0] == 'q')
                return;
                
            if (PCController(screen, input[0])) {
                GetAllNPCMoves(screen, &moveQueue);
                currentPriority = Entities[PC].weightFactor[screen->pc.originalTile.biomeID];
            }
        }

        if (pq_size(&moveQueue) == 0)
            continue;

        void* nextMove;

        do {
            pq_dequeue(&moveQueue, &nextMove);

            entityType_t* npc = (entityType_t*)nextMove;
            vector_t npcMove = { .x = npc->entityPath->coord.x, .y = npc->entityPath->coord.y };

            printf("%d: (%d, %d), %d\n", npc->tile.biomeID, (int)npc->entityPath->coord.x, (int)npc->entityPath->coord.y, moveQueue.array[pq_size(&moveQueue)].priority);

            if(MoveEntity(screen, npc, npcMove) == 0) {
                GetAllNPCMoves(screen, &moveQueue);
                break;
            }

            npc->entityPath = npc->entityPath->previous;
        } while (pq_size(&moveQueue) > 0 && moveQueue.array[pq_size(&moveQueue) - 1].priority <= currentPriority);
    }
}

int main () {
    Initialize();
    
    waves_t waves = GetWaves();

    if(GENERATEPPM)
        GeneratePPM(waves);

    int currX = MIDDLEX;
    int currY = MIDDLEY;

    UpdateOffset(currX, currY);
    screen_t screen;

    if(DEVMODE == 1)
        DevLoop(&screen, waves, currX, currY);

    GameLoop(&screen, waves, currX, currY);
    
    return 0;
}