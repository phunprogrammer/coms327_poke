#include "WandererTile.h"
#include "Screen.h"

WandererTile::WandererTile(Screen& screen) : 
    NPCTile(Entity::WANDERER, screen), direction(randomDirection()) {
    coord_t coord = random();

    this->coord = coord;
    screen.getEntities().insert(coord, this);
}

std::vector<path_t> WandererTile::move() {

}