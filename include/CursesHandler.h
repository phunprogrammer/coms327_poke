#include "Config.h"

#ifndef CURSESHANDLER_H
#define CURSESHANDLER_H

class Screen;

class CursesHandler {
    private:
        Screen& screen;
    public:
        CursesHandler(Screen& screen);
        int PrintScreen();
        int UpdateEntities();
};

#endif