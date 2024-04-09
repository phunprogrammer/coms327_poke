#include "PacerTile.h"
#include "Screen.h"

PacerTile::PacerTile(Screen& screen) : 
    NPCTile(Entity::PACER, screen) {
}

int PacerTile::queueMove() {
    coord_t move = { this->coord.x + this->direction.x, this->coord.y + this->direction.y };

    if(!ValidMove(move)) {
        direction.x *= -1;
        direction.y *= -1;
    }

    screen->pushToQueue(PQItem((EntityTile*)this, screen->getPriority() + this->speed.at((*screen)[move])));
    return 1;
}