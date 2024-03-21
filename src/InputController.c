#include "InputController.h"
#include "EntityMover.h"
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>

#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

int MovePC(screen_t* screen, char input) {
    vector_t move;

    switch(input) {
        case '7':
        case 'q':
            move.x = screen->pc.coord.x - 1;
            move.y = screen->pc.coord.y - 1;
            break;
        case '8':
        case 'w':
            move.x = screen->pc.coord.x;
            move.y = screen->pc.coord.y - 1;
            break;
        case '9':
        case 'e':
            move.x = screen->pc.coord.x + 1;
            move.y = screen->pc.coord.y - 1;
            break;
        case '4':
        case 'a':
            move.x = screen->pc.coord.x - 1;
            move.y = screen->pc.coord.y;
            break;
        case '6':
        case 'd':
            move.x = screen->pc.coord.x + 1;
            move.y = screen->pc.coord.y;
            break;
        case '1':
        case 'z':
            move.x = screen->pc.coord.x - 1;
            move.y = screen->pc.coord.y + 1;
            break;
        case '2':
        case 'x':
            move.x = screen->pc.coord.x;
            move.y = screen->pc.coord.y + 1;
            break;
        case '3':
        case 'c':
            move.x = screen->pc.coord.x + 1;
            move.y = screen->pc.coord.y + 1;
            break;
        default:
            return 0;
    }

    return MoveEntity(screen, &(screen->pc), move);
}

int ListTrainers(screen_t* screen) {
    const int strlen = 23;
    const int listSize = 5;

    WINDOW* menu = newwin(listSize + 2, strlen + 4, MENUMARGIN, MENUMARGIN);

    char menuItems[screen->npcSize][strlen + 1];

    for(int i = 0; i < screen->npcSize; i++) {
        int coordY = screen->npcs[i].coord.y - screen->pc.coord.y;
        int coordX = screen->npcs[i].coord.x - screen->pc.coord.x;
        sprintf(menuItems[i], "%c, %2d %s and %2d %s", screen->npcs[i].tile.type, abs(coordY), coordY > 0 ? "SOUTH" : "NORTH", abs(coordX), coordX > 0 ? "EAST" : "WEST");
    }

    int index = 0;
    int input = 0;

    do {
        switch (input) {
            case KEY_UP:
                index = max(0, index - 1);
                break;
            case KEY_DOWN:
                index = min(screen->npcSize - listSize, index + 1);
                break;
            default:
        }

        wclear(menu);
        box(menu, 0, 0);
        for(int i = index; i < index + listSize; i++) {
            mvwprintw(menu, i - index + 1, 2, "%s", menuItems[i]);
        }
        wrefresh(menu);
    } while((input = getch()) != 27);

    delwin(menu);

    return 1;
}

int EnterBuilding(screen_t* screen) {
    const int strlen = 23;
    const int listSize = 5;
    enum Tile currentTile = screen->pc.originalTile.biomeID;

    if(currentTile != POKEC && currentTile != POKEM) return 0;

    WINDOW* menu = newwin(listSize + 2, strlen + 4, MENUMARGIN, MENUMARGIN);

    int input = 0;

    do {
        wclear(menu);
        box(menu, 0, 0);

        mvwprintw(menu, 1, 2, "This is a %s", currentTile == POKEC ? "Pokecenter" : "Pokemart");

        wrefresh(menu);
    } while((input = getch()) != '<');

    delwin(menu);

    return 1;
}

int EnterBattle(screen_t* screen, int entityIndex) {
    const int strlen = 23;
    const int listSize = 5;
    entityType_t challenger = screen->npcs[entityIndex];

    WINDOW* menu = newwin(listSize + 2, strlen + 4, MENUMARGIN, MENUMARGIN + 10);

    int input = 0;

    do {
        wclear(menu);
        box(menu, 0, 0);

        mvwprintw(menu, 1, 2, "%c wants to battle, (%d,%d)", challenger.tile.type, (int)challenger.coord.x, (int)challenger.coord.y);

        wrefresh(menu);
    } while((input = getch()) != 27);

    delwin(menu);

    return 1;
}

void InitColors() {
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLACK);
    init_pair(8, COLOR_BLUE, COLOR_BLACK);
}

int DefeatedWeight(entityType_t* entity) {
    for(int i = 0; i < TILENUM; i++) {
        entity->weightFactor[i] *= 2;
    }

    return 1;
}

int PrintMap(screen_t* screen, int* line) {
    for(; *line < WIDTH + 2; (*line)++) {
        int x;
        for(x = 0; x < LENGTH; x++) {
            attron(COLOR_PAIR(screen->biomeMap[*line - 2][x].biomeID < BIOMENUM + STRUCNUM ? screen->biomeMap[*line - 2][x].biomeID + 1 : 6));
            mvprintw(*line, x, "%c", screen->biomeMap[*line - 2][x].type);
            attroff(COLOR_PAIR(screen->biomeMap[*line - 2][x].biomeID < BIOMENUM + STRUCNUM ? screen->biomeMap[*line - 2][x].biomeID + 1 : 6));
        }
    }
    mvprintw((*line)++, 0, "(%d, %d)", MIDDLEX, MIDDLEY);

    return 1;
}