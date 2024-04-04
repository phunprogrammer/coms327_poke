#include "PacerTile.h"
#include "Screen.h"

PacerTile::PacerTile(Screen& screen) : 
    NPCTile(Entity::PACER, screen), direction(randomDirection()) {
    coord_t coord = random();

    this->coord = coord;
    screen.getEntities().insert(coord, this);
}

std::vector<path_t> PacerTile::move() {

}