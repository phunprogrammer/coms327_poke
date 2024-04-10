#include <ncurses.h>

#ifndef CURSESHANDLER_H
#define CURSESHANDLER_H

class Screen;
class EntityTile;
class NPCTile;
class PCTile;

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
};

#endif