#include "NoiseGenerator.h"
#include "PPMGenerator.h"
#include "TerrainGenerator.h"
#include "Config.h"
#include "PQueue.h"
#include "EntityGenerator.h"
#include "EntityMover.h"
#include "InputController.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ncurses.h>

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

void CheckQueue(screen_t* screen, pqueue_t* moveQueue, int currentPriority) {
    void* nextMove;

    do {
        pq_dequeue(moveQueue, &nextMove);

        entityType_t* npc = (entityType_t*)nextMove;

        vector_t npcMove = { .x = npc->entityPath->coord.x, .y = npc->entityPath->coord.y };

        // if (npc->tile.biomeID == PACER) {
        //     printf("Hi");
        // }

        //printf("%d: (%d, %d), %d\n", npc->tile.biomeID, (int)npc->entityPath->coord.x, (int)npc->entityPath->coord.y, moveQueue->array[pq_size(moveQueue)].priority);

        if(MoveEntity(screen, npc, npcMove) == 0 && npc->entityPath->previous == NULL) {
            AddPathToQ(moveQueue, screen, npc, currentPriority);
            continue;
        }

        path_t* tempPath = npc->entityPath;
        npc->entityPath = npc->entityPath->previous;
        free(tempPath);
    } while (pq_size(moveQueue) > 0 && moveQueue->array[pq_size(moveQueue) - 1].priority <= currentPriority);
}

void GameLoop(screen_t* screen, waves_t waves, int argc, char *argv[]) {
    *screen = ScreenGenerator(waves);
    InitSize(screen, argc, argv);
    RandomizePC(screen);

    char currInput = 0;
    
    while(currInput != 'Q') {
        for(int y = 0; y < WIDTH; y++)
            for(int x = 0; x < LENGTH; x++)
                mvprintw(y, x, "%c", screen->biomeMap[y][x].type);

        printf("(%d, %d)\n", currX - MIDDLEX, currY - MIDDLEY);
        refresh();

        currInput = getch();

        MovePC(screen, currInput);
    }

    DestroyScreen(screen);
    endwin();
}

int main (int argc, char *argv[]) {
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

    initscr();
    
    GameLoop(&screen, waves, argc, argv);

    return 0;
}