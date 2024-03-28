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
                ScreenGenerator(screen, waves, currX, (currY = fmax(--currY, MINSIZE)));
                break;
            case 'e':
                ScreenGenerator(screen, waves, (currX = fmin(++currX, MAXSIZE)), currY);
                break;
            case 's':
                ScreenGenerator(screen, waves, currX, (currY = fmin(++currY, MAXSIZE)));
                break;
            case 'w':
                ScreenGenerator(screen, waves, (currX = fmax(--currX, MINSIZE)), currY);
                break;
            case 'f':
                int x, y;
                scanf("%d %d", &x, &y);
                ScreenGenerator(screen, waves, (currX = (int)fmax(fmin(MIDDLEX + x, MAXSIZE), MINSIZE)), (currY = (int)fmax(fmin(MIDDLEY + y, MAXSIZE), MINSIZE)));
                break;
            case 0:
                break;
            case '\n':
                continue;
            default:
                printf("ERROR: Not a valid command\n");
                continue;
        }
    
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
    char currInput = 0;
    int updateScreen = 1;
    char cameFrom;
    int hasFlown = 1;
    vector_t screenCoord = { .x = MIDDLEX, .y = MIDDLEY };

    while(currInput != 'Q') {
        if(updateScreen) {
            ScreenGenerator(screen, waves, (int)screenCoord.x, (int)screenCoord.y);
            InitSize(screen, argc, argv);

            if(hasFlown) {
                RandomizePC(screen);
                hasFlown = 0;
            }
            else 
                PlacePC(screen, cameFrom);

            SpawnAllNPC(screen);

            screen->moveQueue = new pqueue_t;
            pq_init(screen->moveQueue);
            screen->priority = 0;
            GetAllNPCMoves(screen, screen->priority);

            updateScreen = 0;
        }
        
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
            case 'f':
                fly(&screenCoord);
                updateScreen = 1;
                hasFlown = 1;
                continue;
            default:
                vector_t move = MovePC(screen, currInput);

                if(move.x < 0) {
                    screenCoord.x = fmax(screen->coord.x - 1, MINSIZE);
                    cameFrom = 'e';
                    updateScreen = 1;
                    continue;
                }

                if (move.x >= LENGTH) {
                    screenCoord.x = fmin(screen->coord.x + 1, MAXSIZE);
                    cameFrom = 'w';
                    updateScreen = 1;
                    continue;
                }

                if (move.y < 0) {
                    screenCoord.y = fmax(screen->coord.y - 1, MINSIZE);
                    cameFrom = 's';
                    updateScreen = 1;
                    continue;
                }

                if (move.y >= WIDTH) {
                    screenCoord.y = fmin(screen->coord.y + 1, MAXSIZE);
                    cameFrom = 'n';
                    updateScreen = 1;
                    continue;
                }

                if((move.x == 0 && move.y == 0) || !MoveEntity(screen, &(screen->pc), move))
                    continue;
        }

        screen->priority += Entities[PC].weightFactor[screen->pc.originalTile.biomeID];
        
        while (!pq_isEmpty(screen->moveQueue)) {
            void* node;
            pq_peek(screen->moveQueue, &node);
            entityMove_t* move = (entityMove_t*)node;

            if(move->priority > screen->priority)
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

    if(DEVMODE == 1)
        DevLoop(&screen, waves, currX, currY);

    initscr();
    noecho();
    curs_set(0);
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    InitColors();
    
    GameLoop(&screen, waves, seed, argc, argv);

    return 0;
}