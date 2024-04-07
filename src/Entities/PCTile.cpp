#include "PCTile.h"
#include "Screen.h"
#include <stdlib.h>

PCTile::PCTile(Screen& screen, coord_t coord) : 
    EntityTile(Entity::PC, coord, screen) {}

int PCTile::move() {

}

void PCTile::setCoord(coord_t coord) {
    this->prevCoord = this->coord;
    screen->getEntities().move(this->coord, coord);
    this->coord = coord;
}

void PCTile::setCoordRandom() {
    this->prevCoord = this->coord;
    coord_t random = randomCoord();
    screen->getEntities().move(this->coord, random);
    this->coord = random;
}