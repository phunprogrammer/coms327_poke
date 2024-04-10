#include "WandererTile.h"
#include "Screen.h"
#include <stdlib.h>

WandererTile::WandererTile(Screen& screen) : 
    NPCTile(Entity::WANDERER, screen) {
}

int WandererTile::queueMove() {
    int count = 0;
    coord_t move = { this->coord.x + this->direction.x, this->coord.y + this->direction.y };

    while((!ValidMove(move) || (*screen)[this->coord] != (*screen)[move]) && count < 20 && (screen->getEntities()[move] == NULL_ENTITY_PTR || screen->getEntities()[move]->getEntity() != Entity::PC)) {
        direction.x = (rand() % 3) - 1;
        direction.y = (direction.x == 0) ? ((rand() % 2 == 0) ? -1 : 1) : (rand() % 3) - 1;
        move = { this->coord.x + this->direction.x, this->coord.y + this->direction.y };
        count++;
    }

    screen->pushToQueue(PQItem((EntityTile*)this, screen->getPriority() + this->speed.at((*screen)[move])));
    return 1;
}