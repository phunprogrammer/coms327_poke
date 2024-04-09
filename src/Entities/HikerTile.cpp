#include "HikerTile.h"
#include "Screen.h"
#include <AStar.h>

HikerTile::HikerTile(Screen& screen) : 
    NPCTile(Entity::HIKER, screen) {
}

int HikerTile::queueMove() {
    float biomeFactor = 1.0;
    int neighbors = 8;
    int biomeGrid[WIDTH][LENGTH] = { };

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            if(y == 0 || x == 0 || y == WIDTH - 1 || x == LENGTH - 1)
                biomeGrid[y][x] = 0;
            else if(screen->getEntities()[{ x, y }] != NULL_ENTITY_PTR)
                biomeGrid[y][x] = 100;
            else
                biomeGrid[y][x] = this->speed.at((*screen)[{ x, y }]);
        }
    }

    biomeGrid[this->coord.y][this->coord.x] = this->speed.at((*screen)[this->coord]);
    std::vector<path_t> paths = aStar(biomeGrid, WIDTH, LENGTH, this->coord.x, this->coord.y, screen->getEntities()[0]->getCoord().x, screen->getEntities()[0]->getCoord().y, biomeFactor, neighbors);
    
    if((int)paths.size() - 1 < 1 || !ValidMove(paths[paths.size() - 1].coord)) {
        this->direction = { 0, 0 };
        screen->pushToQueue(PQItem((EntityTile*)this, screen->getPriority() + this->speed.at((*screen)[this->coord])));
        return 0;
    }

    path_t path = paths[paths.size() - 1];

    this->direction.x = path.coord.x - this->coord.x;
    this->direction.y = path.coord.y - this->coord.y;

    screen->pushToQueue(PQItem((EntityTile*)this, screen->getPriority() + path.gCost));

    return 1;
}