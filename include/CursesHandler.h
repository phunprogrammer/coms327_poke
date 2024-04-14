#include <ncurses.h>

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
        int BattleMenu(WINDOW* menu);
        int FightMenu(WINDOW* menu, Pokemon pokemon);
};

#endif