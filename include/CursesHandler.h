#include <ncurses.h>
#include <vector>
#include <string>
#include "Items.h"

#ifndef CURSESHANDLER_H
#define CURSESHANDLER_H

class Screen;
class EntityTile;
class NPCTile;
class PCTile;
class Pokemon;
struct move_db;

class CursesHandler {
    private:
        int start = 2;
        WINDOW* screenWin;
        Screen& screen;
        void InitColors();
        void BattleGraphics(WINDOW* menu);
        int CalcEscape(int attempts, Pokemon ally, Pokemon enemy);
    public:
        CursesHandler(Screen& screen);
        ~CursesHandler();
        int PrintScreen();
        int UpdateEntity(int index);
        int UpdateEntity(EntityTile* entity);
        int BattleScreen(NPCTile* npc, PCTile* pc);
        int BattleScreen(PCTile* pc);
        int ListTrainers();
        int ChooseStarter();
        int BattleInfo(WINDOW* menu, Pokemon pc, Pokemon enemy);
        int BattleMenu(WINDOW* menu);
        int FightMenu(WINDOW* menu, Pokemon pokemon);
        int PKMNMenu(WINDOW* menu, std::vector<Pokemon> party);
        int BagMenu(Pokemon& enemy, bool wild);
        int UseItem(WINDOW* menu, ItemEnum item, Pokemon& enemy, bool wild);
        int PrintText(WINDOW* window, std::string text);
        int AttackCycle(WINDOW* window, Pokemon& attacker, move_db attackerMove, Pokemon& defender, move_db defenderMove, int turn);
        int Battle(WINDOW* battleWin, WINDOW* menuWin, PCTile* pc, std::vector<Pokemon>& enemyParty, bool wild);
        int HealPoke();
        int EnterMart();
};

#endif