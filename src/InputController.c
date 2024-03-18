#include "InputController.h"
#include "EntityMover.h"

int MovePC(screen_t* screen, char input) {
    vector_t move;

    switch(input) {
        case 'w':
            move.x = screen->pc.coord.x;
            move.y = screen->pc.coord.y - 1;
            break;
        case 'a':
            move.x = screen->pc.coord.x - 1;
            move.y = screen->pc.coord.y;
            break;
        case 's':
            move.x = screen->pc.coord.x;
            move.y = screen->pc.coord.y + 1;
            break;
        case 'd':
            move.x = screen->pc.coord.x + 1;
            move.y = screen->pc.coord.y;
            break;
        default:
            return 0;
    }

    MoveEntity(screen, &(screen->pc), move);
    return 1;
}