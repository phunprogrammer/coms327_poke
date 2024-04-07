#include <ncurses.h>

#ifndef CURSESHANDLER_H
#define CURSESHANDLER_H

class Screen;

class CursesHandler {
    private:
        int start = 2;
        WINDOW* screenWin;
        WINDOW* entityWin;
        Screen& screen;
        int seed;
    public:
        CursesHandler(Screen& screen, int seed);
        int PrintScreen();
        int UpdateEntities();
};

#endif