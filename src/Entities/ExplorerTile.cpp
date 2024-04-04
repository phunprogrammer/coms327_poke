#include "ExplorerTile.h"
#include "Screen.h"

ExplorerTile::ExplorerTile(Screen& screen) : 
    NPCTile(Entity::EXPLORER, screen), direction(randomDirection()) {
    coord_t coord = random();

    this->coord = coord;
    screen.getEntities().insert(coord, this);
}

std::vector<path_t> ExplorerTile::move() {

}