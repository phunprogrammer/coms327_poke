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
            return 1;
        }
    }

    Initialize();
    char current;
    waves = Screen::GetWaves();
    coord_t coord = {200, 200};

    InputHandler input = InputHandler(new Screen(waves, coord, new PCTile({0, 0})));

    do
    {
        flushinp();
        Command command = (Command)input.HandleInput(current);

        if(command == END)
            break;

        if(command == CONTINUE)
            continue;

        while(!input.getScreen()->getMoveQueue().empty()) {
            EntityTile* top = input.getScreen()->getEntityManager().PopTop();

            if(top == NULL_ENTITY_PTR)
                break;
        }
        
        int i = 0;
        move(TERM_WIDTH, 0);
        clrtoeol();
        for (const auto& pair : input.getScreen()->getEntities().getMap())
            mvwprintw(stdscr, TERM_WIDTH, i++, "%c", pair.second->getEntity());

        move(TERM_WIDTH + 1, 0);
        clrtoeol();
        for (i = 0; i < (int)input.getScreen()->getEntities().size(); i++)
             mvwprintw(stdscr, TERM_WIDTH + 1, i, "%c", input.getScreen()->getEntities()[i]->getEntity());

    } while ((current = getch()) != 'Q');

    return 0;
}