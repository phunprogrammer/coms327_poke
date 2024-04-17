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
    setlocale(LC_ALL, "");
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
    init_pair(Structure::PCNTR, COLOR_RED, COLOR_BLACK);
    init_pair(Structure::PMART, COLOR_BLUE, COLOR_BLACK);
    init_pair(Entity::NULL_ENTITY, COLOR_WHITE, COLOR_BLACK);
}

int CursesHandler::BattleScreen(NPCTile* npc, PCTile* pc) {
    PrintScreen();
    int length = std::min(WIDTH * 2, LENGTH);
    WINDOW* battleWin = newwin(WIDTH, length, start, (LENGTH - length) / 2);
    WINDOW* menuWin = newwin(WIDTH * 0.35, length, start + std::ceil(WIDTH * 0.65), (LENGTH - length) / 2);
    box(battleWin, 0, 0);

    std::stringstream string;
    string << npc->getEntity() << " wants to battle! ";
    PrintText(menuWin, string.str());

    int battleWinner = Battle(battleWin, menuWin, pc, npc->getParty(), false);

    if(battleWinner == 0) {
        npc->defeat();
        screen.getEntities().remove(npc->getCoord());

        std::stringstream string;
        string << "You defeated " << npc->getEntity();
        PrintText(menuWin, string.str());
    }

    delwin(battleWin);
    PrintScreen();
    return 1;
}

int CursesHandler::BattleScreen(PCTile* pc) {
    PrintScreen();
    Pokemon pokemon = Pokemon(screen.getCoord());
    std::vector<Pokemon> wild({pokemon});

    int length = std::min(WIDTH * 2, LENGTH);
    WINDOW* battleWin = newwin(WIDTH, length, start, (LENGTH - length) / 2);
    WINDOW* menuWin = newwin(WIDTH * 0.35, length, start + std::ceil(WIDTH * 0.65), (LENGTH - length) / 2);
    box(battleWin, 0, 0);

    std::stringstream string;
    string << "You encountered a wild " << pokemon.getPokemonSpecies().identifier << "!";
    PrintText(menuWin, string.str());

    int battleOutcome = Battle(battleWin, menuWin, pc, wild, true);

    if(battleOutcome == 0) {
        std::stringstream string;
        string << "You defeated " << pokemon.getPokemonSpecies().identifier;
        PrintText(menuWin, string.str());
    }

    delwin(battleWin);
    PrintScreen();

    return 1;
}

int CursesHandler::Battle(WINDOW* battleWin, WINDOW* menuWin, PCTile* pc, std::vector<Pokemon>& enemyParty, bool wild) {
    int pcPokemon;

    for(pcPokemon = 0; pcPokemon < (int)pc->getParty().size(); pcPokemon++)
        if(!pc->getParty()[pcPokemon].isFainted())
            break;

    int enemyPokemon = 0;
    move_db pcMove;
    move_db npcMove;
    int battleOutcome = 0;
    int escapeAttempts = 0;

    while(battleOutcome == 0) {
        int pcTurn = 1;
        int npcTurn = 1;
        wclear(battleWin);

        BattleInfo(battleWin, pc->getParty()[pcPokemon], enemyParty[enemyPokemon]);

        switch(BattleMenu(menuWin)) {
            case 0:
                int moveTemp;
                if((moveTemp = FightMenu(menuWin, pc->getParty()[pcPokemon])) == (int)pc->getParty()[pcPokemon].getLearnedMoves().size()) 
                    continue;
                else
                    pcMove = moves[pc->getParty()[pcPokemon].getLearnedMoves()[moveTemp].move_id];
                break;
            case 1: {
                    int pkmnTemp = 0;
                    if((pkmnTemp = PKMNMenu(menuWin, pc->getParty())) == (int)pc->getParty().size() || pkmnTemp == pcPokemon)
                        continue;
                    else {
                        pcPokemon = pkmnTemp;
                        pcTurn = 0;
                        
                        std::stringstream string;
                        string << "You switched to " << pc->getParty()[pcPokemon].getPokemonSpecies().identifier;
                        PrintText(menuWin, string.str());
                        BattleInfo(battleWin, pc->getParty()[pcPokemon], enemyParty[enemyPokemon]);
                    }
                    break;
                }
                break;
            case 2:
                continue;
            case 3:
                if(wild) {
                    if(rand() % 256 < CalcEscape(escapeAttempts, pc->getParty()[pcPokemon], enemyParty[enemyPokemon])) {
                        PrintText(menuWin, "You escaped!");
                        battleOutcome = 2;
                        continue;
                    }
                    else {
                        PrintText(menuWin, "You failed to escape.");
                        pcTurn = 0;
                    }
                }
                else {
                    PrintText(menuWin, "You cannot run away");
                    continue;
                }
                break;
        }

        npcMove = moves[enemyParty[enemyPokemon].getLearnedMoves()[rand() % enemyParty[enemyPokemon].getLearnedMoves().size()].move_id];

        int pcPriority = pcMove.priority * 1000 + pc->getParty()[pcPokemon].getPokemonStats()[Stat::SPEED];
        int npcPriority = npcMove.priority * 1000 + enemyParty[enemyPokemon].getPokemonStats()[Stat::SPEED];

        int defeat;

        if (pcPriority >= npcPriority && pcTurn) {
            defeat = AttackCycle(menuWin, pc->getParty()[pcPokemon], pcMove, enemyParty[enemyPokemon], npcMove, npcTurn);
            BattleInfo(battleWin, pc->getParty()[pcPokemon], enemyParty[enemyPokemon]);

            if(defeat == 1) {
                std::stringstream string;
                string << enemyParty[enemyPokemon].getPokemonSpecies().identifier << " fainted!";
                PrintText(menuWin, string.str());
            }
        } else if(npcTurn) {
            defeat = AttackCycle(menuWin, enemyParty[enemyPokemon], npcMove, pc->getParty()[pcPokemon], pcMove, pcTurn);
            BattleInfo(battleWin, pc->getParty()[pcPokemon], enemyParty[enemyPokemon]);

            if(defeat == 1) {
                std::stringstream string;
                string << pc->getParty()[pcPokemon].getPokemonSpecies().identifier << " fainted!";
                PrintText(menuWin, string.str());
            }
        }

        if (enemyParty[enemyPokemon].isFainted()) {
            int experience = (enemyParty[enemyPokemon].getPokemonData().base_experience * enemyParty[enemyPokemon].getLevel()) / 7;
            pc->getParty()[pcPokemon].IncrementExp(experience * (wild ? 1 : 1.5));

            std::stringstream string;
            string << pc->getParty()[pcPokemon].getPokemonSpecies().identifier << " gained " << experience << "exp!";
            PrintText(menuWin, string.str());

            if(enemyPokemon == (int)enemyParty.size() - 1)
                break;

            enemyPokemon++;

            string.str(std::string());
            string << "Enemy switched to " << enemyParty[enemyPokemon].getPokemonSpecies().identifier;
            PrintText(menuWin, string.str());
            BattleInfo(battleWin, pc->getParty()[pcPokemon], enemyParty[enemyPokemon]);
        }

        if (pc->getParty()[pcPokemon].isFainted()) {
            if(pc->isDefeated()) {
                PrintText(menuWin, "All of your pokemon have fainted!");
                battleOutcome = 1;
                continue;
            }
            else {
                while((pcPokemon = PKMNMenu(menuWin, pc->getParty())) >= (int)pc->getParty().size());

                std::stringstream string;
                string << "You switched to " << pc->getParty()[pcPokemon].getPokemonSpecies().identifier;
                PrintText(menuWin, string.str());
                BattleInfo(battleWin, pc->getParty()[pcPokemon], enemyParty[enemyPokemon]);
            }
        }
    }

    return battleOutcome;
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

    return 0;
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
            case KEY_BACKSPACE:
                selection = (int)pokemon.getLearnedMoves().size();
                return selection;
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

int CursesHandler::PKMNMenu(WINDOW* menu, std::vector<Pokemon> party) {
    int selection = 0;

    int length = std::min(WIDTH * 2, LENGTH);
    int input = 0;

    do {
        wclear(menu);
        box(menu, 0, 0);

        switch(input) {
            case KEY_DOWN:
                selection = std::min((int)party.size() - 1, selection + 1);
                break;
            case KEY_UP:
                selection = std::max(0, selection - 1);
                break;
            case KEY_BACKSPACE:
                selection = (int)party.size();
                return selection;
        }

        for(int i = 0; i < (int)party.size(); i++) {
            if (party[i].isFainted())
                wattron(menu, COLOR_PAIR(Structure::PMART));
                
            if(selection == i) {
                mvwprintw(menu, (i + 1), length * 0.55 - 1, ">%s<", party[i].getPokemonSpecies().identifier);
            }
            else 
                mvwprintw(menu, (i + 1), length * 0.55, "%s", party[i].getPokemonSpecies().identifier);

            wattroff(menu, COLOR_PAIR(Structure::PMART));
        }

        wrefresh(menu);
    } while((input = getch()) != 10 || party[selection].isFainted());

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

    std::vector<pokemon_species_db> starterPokemon;

    for(int i = 0; i < 3; i++) {
        starterPokemon.push_back(pokemon_species[rand() % (pokemon_species_size - 1) + 1]);
    }

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
                 mvwprintw(starterWin, i * 2 + 3, length / 4, "*%s", starterPokemon[i].identifier);
                 continue;
            }
            mvwprintw(starterWin, i * 2 + 3, length / 4, " %s", starterPokemon[i].identifier);
        }
        wrefresh(starterWin);
    } while((input = getch()));

    return 0;
}

int CursesHandler::BattleInfo(WINDOW* menu, Pokemon pc, Pokemon enemy) {
    wclear(menu);
    BattleGraphics(menu);

    //PC
    mvwaddstr(menu, 9, 18, pc.getPokemonSpecies().identifier);
    char info[48];
    snprintf(info, 48, "lvl:%-4d hp:%d/%d", pc.getLevel(), pc.getHp(), pc.getPokemonStats()[Stat::HP]);
    mvwaddstr(menu, 10, 18, info);

    int hpBar;
    int currentHp = std::ceil(20.0 * ((float)pc.getHp() / pc.getPokemonStats()[Stat::HP]));

    for(hpBar = 0; hpBar < currentHp; hpBar++)
        mvwaddstr(menu, 11, 18 + hpBar, "▮");

    for(; hpBar < 20; hpBar++)
        mvwaddstr(menu, 11, 18 + hpBar, "▯");

    //ENEMY
    mvwaddstr(menu, 2, 3, enemy.getPokemonSpecies().identifier);
    snprintf(info, 48, "lvl:%-4d hp:%d/%d", enemy.getLevel(), enemy.getHp(), enemy.getPokemonStats()[Stat::HP]);
    mvwaddstr(menu, 3, 3, info);

    currentHp = std::ceil(20.0 * ((float)enemy.getHp() / enemy.getPokemonStats()[Stat::HP]));

    for(hpBar = 0; hpBar < currentHp; hpBar++)
        mvwaddstr(menu, 4, 3 + hpBar, "▮");

    for(; hpBar < 20; hpBar++)
        mvwaddstr(menu, 4, 3 + hpBar, "▯");

    wrefresh(menu);
    return 1;
}

void CursesHandler::BattleGraphics(WINDOW* menu) {
    box(menu, 0, 0);

    mvwaddstr(menu, 9, 5, "(\\__/)");
    mvwaddstr(menu, 10, 3, "_ (o'.') __");
    mvwaddstr(menu, 11, 2, "( z(_(\")(\") )");
    mvwaddstr(menu, 12, 2, "╰───────────╯");

    mvwaddstr(menu, 9, 39, "│");
    mvwaddstr(menu, 10, 39, "│");
    mvwaddstr(menu, 11, 39, "│");
    mvwaddstr(menu, 12, 18, "↼────────────────────╯");

    mvwaddstr(menu, 2, 31, "(\\__/)");
    mvwaddstr(menu, 3, 28, "__ (`д´o) _");
    mvwaddstr(menu, 4, 27, "( (\")(\")_)z )");
    mvwaddstr(menu, 5, 27, "╰───────────╯");

    mvwaddstr(menu, 2, 2, "│");
    mvwaddstr(menu, 3, 2, "│");
    mvwaddstr(menu, 4, 2, "│");
    mvwaddstr(menu, 5, 2, "╰────────────────────⇀");
}

int CursesHandler::PrintText(WINDOW* window, std::string text) {
    int height, width;
    coord_t current = { 1, 1 };

    wclear(window);
    box(window, 0, 0);

    getmaxyx(window, height, width);

    for (char c : text) {
        if(current.x >= width - 1)
            current = { 1, current.y + 1 };

        if(current.y >= height - 1)
            break;

        mvwaddch(window, current.y, current.x++, c);
    }

    wrefresh(window);

    while(getch() != 10);

    return 1;
}

int CursesHandler::AttackCycle(WINDOW* window, Pokemon& attacker, move_db attackerMove, Pokemon& defender, move_db defenderMove, int turn) {
    int out = 0;
    std::stringstream string;
    string << attacker.getPokemonSpecies().identifier << " used " << attackerMove.identifier;
    PrintText(window, string.str());

    int attack = attacker.Attack(attackerMove, defender);

    switch (attack) {
        case 3:
            PrintText(window, "It has NO EFFECT!");
            break;
        case 2:
            PrintText(window, "It was SUPER EFFECTIVE!");
            break;
        case 1:
            PrintText(window, "It hit!");
            break;
        case 0:
            PrintText(window, "It missed!");
            break;
    }

    if (!defender.isFainted() && turn) {
        string.str(std::string());
        string << defender.getPokemonSpecies().identifier << " used " << defenderMove.identifier;
        PrintText(window, string.str());
        attack = defender.Attack(defenderMove, attacker);

        switch (attack) {
            case 3:
                PrintText(window, "It has NO EFFECT!");
                break;
            case 2:
                PrintText(window, "It was SUPER EFFECTIVE!");
                break;
            case 1:
                PrintText(window, "It hit!");
                break;
            case 0:
                PrintText(window, "It missed!");
                break;
        }
    }
    else if (defender.isFainted()){
        out = 1;
    }

    return out;
}

int CursesHandler::CalcEscape(int attempts, Pokemon ally, Pokemon enemy) {
    int baseChance = (ally.getPokemonStats()[Stat::SPEED] * 32) / (float)((enemy.getPokemonStats()[Stat::SPEED] / 4) % 256);
    return baseChance + 30 * attempts;
}

int CursesHandler::HealPoke() {
    int length = std::min(WIDTH * 2, LENGTH);
    WINDOW* menuWin = newwin(WIDTH * 0.35, length, start + std::ceil(WIDTH * 0.65), (LENGTH - length) / 2);
    PrintText(menuWin, "Your party was healed");
    PrintScreen();
    return 1;
}