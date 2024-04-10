#include "Config.h"
#include "Screen.h"
#include "AbstractTiles.h"
#include "PCTile.h"
#include <iostream>
#include <ncurses.h>
#include <CursesHandler.h>
#include <InputHandler.h>
#include <chrono>
#include <thread>

int main(int argc, char* argv[])
{
    if (argc == 3 && std::string(argv[1]) == "--trainers") {
        numNPC = std::atoi(argv[2]);
        if (numNPC <= 0) {
            std::cerr << "Error: Invalid number of trainers" << std::endl;
            return 1; // Error, invalid number of trainers
        }
    }

    Initialize();
    char current;
    waves_t waves = Screen::GetWaves();
    coord_t coord = {200, 200};

    Screen screen = Screen(waves, coord, new PCTile(screen, {0, 0}));
    InputHandler input = InputHandler(screen);

    do
    {
        flushinp();
        Command command = (Command)input.HandleInput(current);

        if(command == END)
            break;

        if(command == CONTINUE)
            continue;

        while(!screen.getMoveQueue().empty()) {
            EntityTile* top = screen.getEntityManager().PopTop();

            if(top == NULL_ENTITY_PTR)
                break;
        }
        
        int i = 0;
        move(TERM_WIDTH, 0);
        clrtoeol();
        for (const auto& pair : screen.getEntities().getMap())
            mvwprintw(stdscr, TERM_WIDTH, i++, "%c", pair.second->getEntity());

        move(TERM_WIDTH + 1, 0);
        clrtoeol();
        for (i = 0; i < (int)screen.getEntities().size(); i++)
             mvwprintw(stdscr, TERM_WIDTH + 1, i, "%c", screen.getEntities()[i]->getEntity());

    } while ((current = getch()) != 'Q');

    return 0;
}