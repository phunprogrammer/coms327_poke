#include "CursesHandler.h"
#include "Screen.h"
#include "Config.h"
#include <Tiles.h>

CursesHandler::CursesHandler(Screen& screen, int seed) : screen(screen), seed(seed) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    PrintScreen();
}

int CursesHandler::PrintScreen() {
    screenWin = newwin(WIDTH, LENGTH, start, 0);
    entityWin = newwin(WIDTH, LENGTH, start, 0); 

    mvwprintw(stdscr, 0, 0, "Seed: %d", seed);

    for (int i = 0; i < WIDTH; ++i)
    {
        for (int j = 0; j < LENGTH; ++j)
        {
            if (screen.getStructureMap()[i][j].getStructure() != NULL_STRUCT)
                mvwprintw(screenWin, i, j, "%c", screen.getStructureMap()[i][j].getStructure());
            else
                mvwprintw(screenWin, i, j, "%c", screen.getTerrainMap()[i][j].getTerrain());
        }
    }

    mvwprintw(stdscr, TERM_WIDTH - 1, 0, "(%d, %d)", screen.getCoord().x, screen.getCoord().y);

    refresh();

    return 1;
}

int CursesHandler::UpdateEntities() {
    overwrite(screenWin, entityWin);

    MapVector entities = screen.getEntities();
    
    for (int i = 0; i < (int)entities.size(); i++)
    {
        auto entity = entities[i];
        mvwaddch(entityWin, entity->getCoord().y, entity->getCoord().x, (char)entity->getEntity());
    }

    wrefresh(entityWin);

    return 1;
}