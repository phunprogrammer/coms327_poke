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

void GameLoop(screen_t* screen, waves_t waves, int argc, char *argv[]) {
    *screen = ScreenGenerator(waves);
    InitSize(screen, argc, argv);
    RandomizePC(screen);
    SpawnNPC(screen, PACER);
    SpawnNPC(screen, RIVAL);

    pqueue_t moveQueue;
    pq_init(&moveQueue);

    int currentPriority = 0;
    char currInput = 0;
    
    while(currInput != 'Q') {
        int line;
        for(line = 0; line < WIDTH; line++) {
            int x;
            for(x = 0; x < LENGTH; x++) {
                mvprintw(line, x, "%c", screen->biomeMap[line][x].type);
                mvprintw(WIDTH, x, "%d", x);
            }
            mvprintw(line, x, "%d", line);
        }
        line++;
        mvprintw(line++, 0, "(%d, %d)", MIDDLEX, MIDDLEY);

        // path_t* path = GetRivalPath(screen, &screen->npcs[0]);

        // while (path != NULL) {
        //     mvprintw(line++, 0, "Path: (%f, %f), Score: %d", path->coord.x, path->coord.y, path->gCost);
        //     path = path->previous;
        // }
        
        // free(path);

        // void* node = NULL;

        // while(!pq_isEmpty(&moveQueue)) {
        //     pq_dequeue(&moveQueue, &node);
        //     entityMove_t* move = (entityMove_t*)node;
        //     mvprintw(line++, 0, "%d: (%f, %f), Score: %d", move->entityIndex, move->coord.x, move->coord.y, moveQueue.array[pq_size(&moveQueue)].priority);
        // }

        currInput = getch();
        clear();

        if(!MovePC(screen, currInput))
            continue;

        GetAllNPCMoves(screen, &moveQueue, currentPriority);

        currentPriority += Entities[PC].weightFactor[screen->pc.originalTile.biomeID];
        
        while (!pq_isEmpty(&moveQueue)) {
            void* node;
            pq_peek(&moveQueue, &node);
            entityMove_t* move = (entityMove_t*)node;

            if(move->priority > currentPriority)
                break;

            pq_dequeue(&moveQueue, &node);
            
            if(!MoveEntity(screen, &screen->npcs[move->entityIndex], move->coord))
                AddPathToQ(&moveQueue, screen, move->entityIndex, move->priority);
        }
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
    noecho();
    
    GameLoop(&screen, waves, argc, argv);

    return 0;
}