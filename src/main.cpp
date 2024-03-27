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
                UpdateOffset(screen, currX, (currY = fmax(--currY, MINSIZE)));
                break;
            case 'e':
                UpdateOffset(screen, (currX = fmin(++currX, MAXSIZE)), currY);
                break;
            case 's':
                UpdateOffset(screen, currX, (currY = fmin(++currY, MAXSIZE)));
                break;
            case 'w':
                UpdateOffset(screen, (currX = fmax(--currX, MINSIZE)), currY);
                break;
            case 'f':
                int x, y;
                scanf("%d %d", &x, &y);
                UpdateOffset(screen, (currX = (int)fmax(fmin(MIDDLEX + x, MAXSIZE), MINSIZE)), (currY = (int)fmax(fmin(MIDDLEY + y, MAXSIZE), MINSIZE)));
                break;
            case 0:
                break;
            case '\n':
                continue;
            default:
                printf("ERROR: Not a valid command\n");
                continue;
        }
        
        ScreenGenerator(screen, waves);
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

void GameLoop(screen_t* screen, waves_t waves, int seed, int argc, char *argv[]) {
    ScreenGenerator(screen, waves);
    InitSize(screen, argc, argv);
    RandomizePC(screen);
    SpawnAllNPC(screen);

    screen->moveQueue = new pqueue_t;
    pq_init(screen->moveQueue);

    int currentPriority = 0;
    char currInput = 0;

    GetAllNPCMoves(screen, currentPriority);
    
    while(currInput != 'Q') {
        clear();
        mvprintw(0, 0, "Seed: %d", seed);
        int line = 2;
        PrintMap(screen, &line);

        currInput = getch();

        switch(currInput) {
            case 't':
                ListTrainers(screen);
                continue;
            case '>':
                EnterBuilding(screen);
                continue;
            case '5':
            case 's':
                break;
            default:
                if(!MovePC(screen, currInput))
                    continue;
        }

        currentPriority += Entities[PC].weightFactor[screen->pc.originalTile.biomeID];
        
        while (!pq_isEmpty(screen->moveQueue)) {
            void* node;
            pq_peek(screen->moveQueue, &node);
            entityMove_t* move = (entityMove_t*)node;

            if(move->priority > currentPriority)
                break;

            pq_dequeue(screen->moveQueue, &node);

            int entityMove = MoveEntity(screen, &screen->npcs[move->entityIndex], move->coord);

            line = 2;
            PrintMap(screen, &line);
            refresh();
            entityType_t* currEntity = &screen->npcs[move->entityIndex];

            if (entityMove == 2 && !currEntity->defeated) {
                EnterBattle(screen, move->entityIndex);
                currEntity->defeated = 1;
                SwitchTile(&(screen->biomeMap[(int)currEntity->coord.y][(int)currEntity->coord.x]), Tiles[currEntity->originalTile.biomeID]);
            }

            if(!currEntity->defeated)
                AddPathToQ(screen, move->entityIndex, move->priority);

            free(node);
        }
    }

    DestroyScreen(screen);
    endwin();
}

int main (int argc, char *argv[]) {
    Initialize();

    int seed;
    waves_t waves = GetWaves(&seed);

    if(GENERATEPPM)
        GeneratePPM(waves);

    int currX = MIDDLEX;
    int currY = MIDDLEY;

    screen_t screen;
    UpdateOffset(&screen, currX, currY);

    if(DEVMODE == 1)
        DevLoop(&screen, waves, currX, currY);

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    InitColors();
    
    GameLoop(&screen, waves, seed, argc, argv);

    return 0;
}