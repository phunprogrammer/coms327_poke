#include <ncurses.h>
#include <vector>

#ifndef CURSESHANDLER_H
#define CURSESHANDLER_H

class Screen;
class EntityTile;
class NPCTile;
class PCTile;
class Pokemon;

class CursesHandler {
    private:
        int start = 2;
        WINDOW* screenWin;
        Screen& screen;
        void InitColors();
        void BattleGraphics(WINDOW* menu);
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
};

#endif