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
    // db_parse(0);
    // Initialize();
    // Pokemon mon1 = Pokemon(pokemon_species[6], 40);
    // Pokemon mon2 = Pokemon(pokemon_species[3], 40);
    // std::cout << mon1.toString() <<std::endl;
    // std::cout << mon2.toString() <<std::endl;
    // mon1.Attack(moves[mon1.getLearnedMoves()[0].move_id], mon2);
    // mon2.Attack(moves[mon2.getLearnedMoves()[3].move_id], mon1);
    // std::cout << mon1.toString() <<std::endl;
    // std::cout << mon2.toString() <<std::endl;

    // return 0;

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
    pc->addToParty(Pokemon(pokemon_species[input.getScreen()->getCursesHandler().ChooseStarter()], 5));
    pc->addToParty(Pokemon(pokemon_species[1], 5));
    pc->addToParty(Pokemon(pokemon_species[2], 5));
    pc->addToParty(Pokemon(pokemon_species[3], 5));
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
            command = (Command)input.getScreen()->getEntityManager().PopTop();

            if(command == END)
                break;
        }
    } while ((current = getch()) != 'Q');

    return 0;
}