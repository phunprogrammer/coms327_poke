#include "Config.h"
#include "Screen.h"
#include "AbstractTiles.h"
#include "PCTile.h"
#include <iostream>
#include <ncurses.h>
#include <CursesHandler.h>

int main()
{
    Initialize();

    int seed;
    char current;
    waves_t waves = Screen::GetWaves(&seed);
    coord_t coord = {200, 200};

    Screen screen = Screen(waves, coord, new PCTile(screen, {0, 0}));

    CursesHandler curses = CursesHandler(screen, seed);

    do
    {
        switch(current) {
            case 'w':
                PCTile* pc = (PCTile*)screen.getEntities()[0];
                pc->setCoord({pc->getCoord().x, pc->getCoord().y - 1});
                break;
        }

        curses.UpdateEntities();
    } while ((current = getch()) != 'Q');

    endwin();

    return 0;
}