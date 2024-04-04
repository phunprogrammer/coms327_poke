#include "SentryTile.h"
#include "Screen.h"

SentryTile::SentryTile(Screen& screen) : 
    NPCTile(Entity::SENTRY, screen) {
    coord_t coord = random();

    this->coord = coord;
    screen.getEntities().insert(coord, this);
}

std::vector<path_t> SentryTile::move() {

}