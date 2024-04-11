#include "Config.h"
#include "Screen.h"
#include "AbstractTiles.h"
#include "PCTile.h"
#include "Pokemon.h"
#include <iostream>
#include <ncurses.h>
#include <CursesHandler.h>
#include <InputHandler.h>

int main(int argc, char* argv[])
{
    if (argc == 3 && std::string(argv[1]) == "--trainers") {
        numNPC = std::atoi(argv[2]);
        if (numNPC < 0) {
            std::cerr << "Error: Invalid number of trainers" << std::endl;
            return 1;
        }
    }

    Initialize();
    db_parse(0);

    char current;
    waves = Screen::GetWaves();
    coord_t coord = {200, 200};

    PCTile* pc = new PCTile({0, 0});
    InputHandler input = InputHandler(new Screen(waves, coord, pc));
    pc->addToParty(Pokemon(pokemon[input.getScreen()->getCursesHandler().ChooseStarter()], 5));
    input.getScreen()->getCursesHandler().PrintScreen();

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
    } while ((current = getch()) != 'Q');

    return 0;
}