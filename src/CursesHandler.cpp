#include "CursesHandler.h"
#include "Screen.h"
#include "Config.h"
#include <Tiles.h>
#include "db_parse.h"
#include <string>
#include <sstream>
#include "Pokemon.h"
#include "PCTile.h"
#include <cmath>

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
    wclear(stdscr);
    mvwprintw(stdscr, 0, 0, "Seed: %d", seed);  

    // for(int i = 0; i < LENGTH; i++) {
    //     mvwprintw(stdscr, 1, i, "%d", i % 10);  
    // }

    // for(int i = 0; i < WIDTH; i++) {
    //     mvwprintw(stdscr, i + 2, LENGTH, "%d", i % 10);  
    // }

    move(1, 0);
    clrtoeol();
    for (const auto& pokemon : screen.getEntities()[0]->getParty()) {
        wprintw(stdscr, "%s ", pokemon.getPokemonSpecies().identifier);
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

    mvwprintw(stdscr, TERM_WIDTH - 1, 0, "(%d, %d)", screen.getCoord().x - MIDDLEX, screen.getCoord().y - MIDDLEY);

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
    WINDOW* menuWin = newwin(WIDTH * 0.35, length, start + std::ceil(WIDTH * 0.65), (LENGTH - length) / 2);
    box(battleWin, 0, 0);

    int input = 0;

    while(1) {
        wclear(battleWin);

        box(battleWin, 0, 0);

        wrefresh(battleWin);
        switch(BattleMenu(menuWin)) {
            case 0:
                FightMenu(menuWin, pc->getParty()[0]);
            case 1:
            case 2:
            case 3:
                break;
        }
        getch();
    }
}

int CursesHandler::BattleScreen(PCTile* pc) {
    Pokemon pokemon = Pokemon(screen.getCoord());
    int length = std::min(WIDTH * 2, LENGTH);
    WINDOW* battleWin = newwin(WIDTH, length, start, (LENGTH - length) / 2);
    box(battleWin, 0, 0);

    mvwprintw(battleWin, 1, 2, "%s wants to battle!\n", pokemon_species[pokemon.getPokemonData().species_id].identifier);
    mvwprintw(battleWin, 2, 2, "Press enter to capture!\n");

    std::istringstream iss(pokemon.toString());
    std::string line;
    int lineNum = 4;
    while (std::getline(iss, line)) {
        mvwprintw(battleWin, lineNum++, 2, "%s", line.c_str());
    }

    wrefresh(battleWin);
    while(getch() != 10) {}
    wclear(battleWin);
    box(battleWin, 0, 0);

    if(rand() % 100 < 50) {
        mvwprintw(battleWin, 1, 2, "It got away! :(, press enter to continue.");
        wrefresh(battleWin);
        while(getch() != 10) {}
        PrintScreen();
        return 0;
    }

    mvwprintw(battleWin, 1, 2, "You caught it!, press enter to continue.");
    pc->addToParty(pokemon);
    wrefresh(battleWin);
    while(getch() != 10) {}
    delwin(battleWin);
    PrintScreen();
    return 1;
}

int CursesHandler::BattleMenu(WINDOW* menu) {
    std::vector<std::vector<std::string>> options{
        {"FIGHT", "PKMN"},
        {"BAG", "RUN"}
    };

    coord_t selection = { 0, 0 };

    int length = std::min(WIDTH * 2, LENGTH);
    int input = 0;

    do {
        wclear(menu);
        box(menu, 0, 0);

        switch(input) {
            case KEY_RIGHT:
                selection.x = std::min((int)options[0].size() - 1, selection.x + 1);
                break;
            case KEY_LEFT:
                selection.x = std::max(0, selection.x - 1);
                break;
            case KEY_DOWN:
                selection.y = std::min((int)options.size() - 1, selection.y + 1);
                break;
            case KEY_UP:
                selection.y = std::max(0, selection.y - 1);
                break;
        }

        for(int i = 0; i < (int)options.size(); i++)
            for(int j = 0; j < (int)options[0].size(); j++) {
                if(selection.x == j && selection.y == i)
                    mvwprintw(menu, 2 * (i + 1), length * 0.55 + j * 10 - 1, ">%s<", options[i][j].c_str());
                else
                    mvwprintw(menu, 2 * (i + 1), length * 0.55 + j * 10, "%s", options[i][j].c_str());
            }

        wrefresh(menu);
    } while((input = getch()) != 10);

    if(options[selection.y][selection.x] == "FIGHT")
        return 0;

    if(options[selection.y][selection.x] == "PKMN")
        return 1;

    if(options[selection.y][selection.x] == "BAG")
        return 2;

    if(options[selection.y][selection.x] == "RUN")
        return 3;
}

int CursesHandler::FightMenu(WINDOW* menu, Pokemon pokemon) {
    int selection = 0;

    int length = std::min(WIDTH * 2, LENGTH);
    int input = 0;

    do {
        wclear(menu);
        box(menu, 0, 0);

        switch(input) {
            case KEY_DOWN:
                selection = std::min((int)pokemon.getLearnedMoves().size() - 1, selection + 1);
                break;
            case KEY_UP:
                selection = std::max(0, selection - 1);
                break;
        }

        for(int i = 0; i < (int)pokemon.getLearnedMoves().size(); i++)
            if(selection == i)
                mvwprintw(menu, (i + 1), length * 0.55 - 1, ">%s<", moves[pokemon.getLearnedMoves()[i].move_id].identifier);
            else
                mvwprintw(menu, (i + 1), length * 0.55, "%s", moves[pokemon.getLearnedMoves()[i].move_id].identifier);

        wrefresh(menu);
    } while((input = getch()) != 10);

    return selection;
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

int CursesHandler::ChooseStarter() {
    const int width = WIDTH / 2;
    const int length = LENGTH / 2;
    WINDOW* starterWin = newwin(width, length, start + (WIDTH - width) / 2, (LENGTH - length) / 2);

    std::vector<pokemon_db> starterPokemon;

    for(int i = 0; i < 3; i++)
        starterPokemon.push_back(pokemon[(rand() % pokemon_db_size - 1) + 1]);


    int selected = 0;
    int input = 0;

    do {
        wclear(starterWin);
        box(starterWin, 0, 0);
        mvwprintw(starterWin, 1, 4, "Use arrow keys, enter to select");

        switch (input) {
            case KEY_UP:
                selected = (selected - 1 + 3) % 3;
                break;
            case KEY_DOWN:
                selected = (selected + 1) % 3;
                break;
            case 10:
                delwin(starterWin);
                PrintScreen();
                return starterPokemon[selected].id;
                break;
        }

        for (int i = 0; i < 3; ++i) {
            if (i == selected) {
                 mvwprintw(starterWin, i * 2 + 3, length / 4, "*%s", pokemon_species[starterPokemon[i].species_id].identifier);
                 continue;
            }
            mvwprintw(starterWin, i * 2 + 3, length / 4, " %s", pokemon_species[starterPokemon[i].species_id].identifier);
        }
        wrefresh(starterWin);
    } while((input = getch()));

    return 0;
}