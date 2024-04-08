#include "CursesHandler.h"
#include "Screen.h"
#include "Config.h"
#include <Tiles.h>

CursesHandler::CursesHandler(Screen& screen, int seed) : screen(screen), seed(seed) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();

    InitColors();
    PrintScreen();
}

int CursesHandler::PrintScreen() {
    screenWin = newwin(WIDTH, LENGTH, start, 0);

    mvwprintw(stdscr, 0, 0, "Seed: %d", seed);  

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

    wattron(screenWin, COLOR_PAIR(Entity::NULL_ENTITY));
    mvwaddch(screenWin, entity->getCoord().y, entity->getCoord().x, (char)entity->getEntity());
    wattroff(screenWin, COLOR_PAIR(Entity::NULL_ENTITY));

    wattron(screenWin, COLOR_PAIR(print));
    mvwaddch(screenWin, entity->getPrevCoord().y, entity->getPrevCoord().x, print);
    wattroff(screenWin, COLOR_PAIR(print));

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