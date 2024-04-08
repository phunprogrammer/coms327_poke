#include "Config.h"
#include "Screen.h"
#include "AbstractTiles.h"
#include "PCTile.h"
#include <iostream>
#include <ncurses.h>
#include <CursesHandler.h>
#include <InputHandler.h>

int main()
{
    Initialize();

    int seed;
    char current;
    waves_t waves = Screen::GetWaves(&seed);
    coord_t coord = {200, 200};

    Screen screen = Screen(waves, coord, new PCTile(screen, {0, 0}));
    CursesHandler curses = CursesHandler(screen, seed);
    InputHandler input = InputHandler(screen, curses);

    do
    {
        int inputOut = input.HandleInput(current);

        if(inputOut == 0)
            continue;

        screen.getMoveQueue().top().getData()->move();
        screen.getMoveQueue().pop();
        curses.UpdateEntity(1);
    } while ((current = getch()) != 'Q');

    endwin();

    return 0;
}