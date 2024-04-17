#include "Config.h"
#include "MapVector.h"

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

class Screen;
class CursesHandler;
class EntityTile;

class InputHandler {
    private:
        std::vector<std::vector<MapVector<coord_t, EntityTile*>>> mapGrid;
        char lastInput;
        Screen* screen;
    public:
        ~InputHandler();
        InputHandler(Screen* screen);
        int HandleInput(char input);
        int MovePC(char input);
        int MoveScreen(char compass);
        int MoveScreen(coord_t coord);
        int Fly();
        int EnterPokeCenter();
        int EnterPokeMart();

        Screen* getScreen() const { return screen; }
};

#endif