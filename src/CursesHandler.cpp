#include "CursesHandler.h"
#include "Screen.h"
#include "Config.h"
#include <Tiles.h>

CursesHandler::CursesHandler(Screen& screen) : screen(screen) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    InitColors();
}

CursesHandler::~CursesHandler() {
    endwin();
}

int CursesHandler::PrintScreen() {
    screenWin = newwin(WIDTH, LENGTH, start, 0);

    mvwprintw(stdscr, 0, 0, "Seed: %d", seed);  

    for(int i = 0; i < LENGTH; i++) {
        mvwprintw(stdscr, 1, i, "%d", i % 10);  
    }

    for(int i = 0; i < WIDTH; i++) {
        mvwprintw(stdscr, i + 2, LENGTH, "%d", i % 10);  
    }

    for (int i = 0; i < WIDTH; ++i)
    {
        for (int j = 0; j < LENGTH; ++j)
        {
            char print = screen[{ j , i }];

            wattron(screenWin, COLOR_PAIR(print));
            mvwprintw(screenWin, i, j, "%c", print);
            wattroff(screenWin, COLOR_PAIR(print));
        }
    }

    wattron(screenWin, COLOR_PAIR(Entity::NULL_ENTITY));
    for(int i = 0; i < (int)screen.getEntities().size(); i++) {
        mvwprintw(screenWin, screen.getEntities()[i]->getCoord().y, screen.getEntities()[i]->getCoord().x, "%c", screen.getEntities()[i]->getEntity());
    }
    wattroff(screenWin, COLOR_PAIR(Entity::NULL_ENTITY));

    mvwprintw(stdscr, TERM_WIDTH - 1, 0, "(%d, %d)", screen.getCoord().x, screen.getCoord().y);

    refresh();
    wrefresh(screenWin);

    return 1;
}

int CursesHandler::UpdateEntity(int index) {
    EntityTile* entity = screen.getEntities()[index];

    return UpdateEntity(entity);
}

int CursesHandler::UpdateEntity(EntityTile* entity) {
    char print = screen[entity->getPrevCoord()];

    wattron(screenWin, COLOR_PAIR(print));
    mvwaddch(screenWin, entity->getPrevCoord().y, entity->getPrevCoord().x, print);
    wattroff(screenWin, COLOR_PAIR(print));

    wattron(screenWin, COLOR_PAIR(Entity::NULL_ENTITY));
    mvwaddch(screenWin, entity->getCoord().y, entity->getCoord().x, (char)entity->getEntity());
    wattroff(screenWin, COLOR_PAIR(Entity::NULL_ENTITY));

    wrefresh(screenWin);

    return 1;
}

void CursesHandler::InitColors() {
    init_pair(Terrain::FOREST, COLOR_BLACK, COLOR_GREEN);
    init_pair(Terrain::MOUNTAIN, COLOR_BLACK, COLOR_WHITE);
    init_pair(Terrain::CLEARING, COLOR_YELLOW, COLOR_BLACK);
    init_pair(Terrain::GRASSLAND, COLOR_GREEN, COLOR_BLACK);
    init_pair(Terrain::OCEAN, COLOR_BLACK, COLOR_CYAN);
    init_pair(Structure::PATH, COLOR_WHITE, COLOR_BLACK);
    init_pair(Structure::PMART, COLOR_RED, COLOR_BLACK);
    init_pair(Structure::PCNTR, COLOR_BLUE, COLOR_BLACK);
    init_pair(Entity::NULL_ENTITY, COLOR_WHITE, COLOR_BLACK);
}

int CursesHandler::BattleScreen(NPCTile* npc, PCTile* pc) {
    int length = std::min(WIDTH * 2, LENGTH);
    WINDOW* battleWin = newwin(WIDTH, length, start, (LENGTH - length) / 2);
    box(battleWin, 0, 0);

    int input = 0;

    while(1) {
        wclear(battleWin);

        if(input == 'Q') {
            delwin(battleWin);
            npc->defeat();
            screen.getEntities().remove(npc->getCoord());
            PrintScreen();
            return CONTINUE;
        }

        box(battleWin, 0, 0);

        mvwprintw(battleWin, 1, 2, "%c wants to battle!", npc->getEntity());

        wrefresh(battleWin);
        input = getch();
    }
}

int CursesHandler::ListTrainers() {
    const int strlen = 23;
    const int listSize = 5;
    WINDOW* trainerWin = newwin(listSize + 2, strlen + 4, start + (WIDTH - (listSize + 2)) / 2, (LENGTH - 27) / 2);

    char menuItems[(int)screen.getEntities().size() - 1][strlen + 1];

    for(int i = 0; i < (int)screen.getEntities().size() - 1; i++) {
        int coordY = screen.getEntities()[i + 1]->getCoord().y - screen.getEntities()[0]->getCoord().y;
        int coordX = screen.getEntities()[i + 1]->getCoord().x - screen.getEntities()[0]->getCoord().x;
        sprintf(menuItems[i], "%c, %2d %s and %2d %s", screen.getEntities()[i + 1]->getEntity(), abs(coordY), coordY > 0 ? "SOUTH" : "NORTH", abs(coordX), coordX > 0 ? "EAST" : "WEST");
    }

    int index = 0;
    int input = 0;

    do {
        switch (input) {
            case KEY_UP:
                index = std::max(0, index - 1);
                break;
            case KEY_DOWN:
                index = std::min((int)screen.getEntities().size() - 1 - listSize, index + 1);
                break;
            default:
                break;
        }

        wclear(trainerWin);
        box(trainerWin, 0, 0);
        for(int i = index; i < index + listSize; i++) {
            mvwprintw(trainerWin, i - index + 1, 2, "%s", menuItems[i]);
        }
        wrefresh(trainerWin);
    } while((input = getch()) != 'Q');

    delwin(trainerWin);
    PrintScreen();

    return 1;
}