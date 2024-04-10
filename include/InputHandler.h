#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

class Screen;
class CursesHandler;

class InputHandler {
    private:
        char lastInput;
        Screen& screen;
    public:
        InputHandler(Screen& screen);
        int HandleInput(char input);
        int MovePC(char input);
};

#endif