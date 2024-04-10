#include "InputHandler.h"
#include "Screen.h"
#include "CursesHandler.h"
#include "PCTile.h"
#include <cstdlib>

InputHandler::InputHandler(Screen& screen) : screen(screen) {}

int InputHandler::HandleInput(char input) {
    switch(input) {
        case 'q':
        case '7':
        case 'w':
        case '8':
        case 'e':
        case '9':
        case 'a':
        case '4':
        case 'd':
        case '6':
        case 'z':
        case '1':
        case 'x':
        case '2':
        case 'c':
        case '3':
        case 's':
        case '5':
            return MovePC(input);
        case 't':
            return screen.getCursesHandler().ListTrainers();
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
        case 's':
        case '5':
            pc->setDirection({ 0, 0 });
            break;
    }

    int out;
    
    out = pc->move();

    return out;
}