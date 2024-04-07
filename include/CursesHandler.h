#include <ncurses.h>

#ifndef CURSESHANDLER_H
#define CURSESHANDLER_H

class Screen;
class EntityTile;

class CursesHandler {
    private:
        int start = 2;
        WINDOW* screenWin;
        Screen& screen;
        int seed;
        void InitColors();
    public:
        CursesHandler(Screen& screen, int seed);
        int PrintScreen();
        int UpdateEntity(int index);
        int UpdateEntity(EntityTile* entity);
};

#endif