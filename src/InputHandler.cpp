#include "InputHandler.h"
#include "Screen.h"
#include "CursesHandler.h"
#include "PCTile.h"

InputHandler::InputHandler(Screen& screen, CursesHandler& cursesHandler) : screen(screen), cursesHandler(cursesHandler) {}

int InputHandler::HandleInput(char input) {
    switch(input) {
        case 'q':
        case 'w':
        case 'e':
        case 'a':
        case 'd':
        case 'z':
        case 'x':
        case 'c':
            return MovePC(input);
        case 's':
            return 1;
    }

    return 0;
}

int InputHandler::MovePC(char input) {
    PCTile* pc = (PCTile*)screen.getEntities()[0];

    switch(input) {
        case 'q':
        case '7':
            pc->setDirection({ -1, -1 });
            break;
        case 'w':
        case '8':
            pc->setDirection({ 0, -1 });
            break;
        case 'e':
        case '9':
            pc->setDirection({ 1, -1 });
            break;
        case 'a':
        case '4':
            pc->setDirection({ -1, 0 });
            break;
        case 'd':
        case '6':
            pc->setDirection({ 1, 0 });
            break;
        case 'z':
        case '1':
            pc->setDirection({ -1, 1 });
            break;
        case 'x':
        case '2':
            pc->setDirection({ 0, 1 });
            break;
        case 'c':
        case '3':
            pc->setDirection({ 1, 1 });
            break;
    }

    int out = pc->move();
    cursesHandler.UpdateEntity(0);

    return out;
}