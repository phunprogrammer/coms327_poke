#include "Config.h"
#include "Screen.h"
#include "AbstractTiles.h"
#include "PCTile.h"
#include <iostream>
#include <ncurses.h>
#include <CursesHandler.h>
#include <InputHandler.h>

int main(int argc, char* argv[])
{
    if (argc == 3 && std::string(argv[1]) == "--trainers") {
        numNPC = std::atoi(argv[2]);
        if (numNPC <= 0) {
            std::cerr << "Error: Invalid number of trainers" << std::endl;
            return 1; // Error, invalid number of trainers
        }
    }

    int seed = 0;
    Initialize(seed);
    char current;
    waves_t waves = Screen::GetWaves();
    coord_t coord = {200, 200};

    Screen screen = Screen(waves, coord, new PCTile(screen, {0, 0}));
    CursesHandler curses = CursesHandler(screen, seed);
    InputHandler input = InputHandler(screen, curses);

    do
    {
        int inputOut = input.HandleInput(current);

        if(inputOut == 0)
            continue;

        while(!screen.getMoveQueue().empty()) {
            EntityTile* top = screen.getEntityManager().PopTop();

            if(top == NULL_ENTITY_PTR) {
                break;
            }

            curses.UpdateEntity(top);
        }
        
    } while ((current = getch()) != 'Q');

    endwin();

    return 0;
}