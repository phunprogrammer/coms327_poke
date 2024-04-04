#include "RivalTile.h"
#include "Screen.h"

RivalTile::RivalTile(Screen& screen) : 
    NPCTile(Entity::RIVAL, screen) {
    coord_t coord = random();

    this->coord = coord;
    screen.getEntities().insert(coord, this);
}

std::vector<path_t> RivalTile::move() {

}