#include "HikerTile.h"
#include "Screen.h"

HikerTile::HikerTile(Screen& screen) : 
    NPCTile(Entity::HIKER, screen) {
    coord_t coord = random();

    this->coord = coord;
    screen.getEntities().insert(coord, this);
}

std::vector<path_t> HikerTile::move() {

}