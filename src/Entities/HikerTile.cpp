#include "HikerTile.h"
#include "Screen.h"
#include <AStar.h>

HikerTile::HikerTile(Screen& screen) : 
    NPCTile(Entity::HIKER, screen) {
}

int HikerTile::queueMove() {
    float biomeFactor = 1.0;
    int neighbors = 8;
    int biomeGrid[WIDTH][LENGTH];

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = this->speed.at((*screen)[{ x, y }]);
        }
    }

    biomeGrid[this->coord.y][this->coord.x] = this->speed.at((*screen)[this->coord]);
    std::vector<path_t> paths = aStar(biomeGrid, WIDTH, LENGTH, this->coord.x, this->coord.y, screen->getEntities()[0]->getCoord().x, screen->getEntities()[0]->getCoord().y, biomeFactor, neighbors);
    path_t path = paths[paths.size() - 1];

    if(!ValidMove(path.coord)) {
        screen->pushToQueue(PQItem((EntityTile*)this, screen->getPriority() + this->speed.at((*screen)[this->coord])));
        return 0;
    }

    this->direction.x = path.coord.x - this->coord.x;
    this->direction.y = path.coord.y - this->coord.y;

    screen->pushToQueue(PQItem((EntityTile*)this, screen->getPriority() + path.gCost));

    return 1;
}