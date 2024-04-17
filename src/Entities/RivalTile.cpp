#include "RivalTile.h"
#include "Screen.h"
#include <AStar.h>

RivalTile::RivalTile(Screen& screen) : 
    NPCTile(Entity::RIVAL, screen) {
}

int RivalTile::queueMove() {
    float biomeFactor = 1.0;
    int neighbors = 8;
    int biomeGrid[WIDTH][LENGTH] = { };

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            if(screen->getEntities()[{ x, y }] != NULL_ENTITY_PTR)
                biomeGrid[y][x] = 100;
            else
                biomeGrid[y][x] = this->speed.at((*screen)[{ x, y }]);
        }
    }

    biomeGrid[this->coord.y][this->coord.x] = this->speed.at((*screen)[this->coord]);
    std::vector<path_t> paths = aStar(biomeGrid, WIDTH, LENGTH, this->coord.x, this->coord.y, screen->getEntities()[0]->getCoord().x, screen->getEntities()[0]->getCoord().y, biomeFactor, neighbors);

    if((int)paths.size() - 1 < 0) {
        this->direction = { 0, 0 };
        screen->pushToQueue(PQItem((EntityTile*)this, screen->getPriority() + this->speed.at((*screen)[this->coord])));
        return 0;
    }

    path_t path = paths[paths.size() - 1];

    this->direction.x = path.coord.x - this->coord.x;
    this->direction.y = path.coord.y - this->coord.y;

    screen->pushToQueue(PQItem((EntityTile*)this, screen->getPriority() + this->speed.at((*screen)[this->coord])));

    return 1;
}