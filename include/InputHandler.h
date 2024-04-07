#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

class Screen;
class CursesHandler;

class InputHandler {
    private:
        char lastInput;
        Screen& screen;
        CursesHandler& cursesHandler;
    public:
        InputHandler(Screen& screen, CursesHandler& cursesHandler);
        int HandleInput(char input);
        int MovePC(char input);

};

#endif