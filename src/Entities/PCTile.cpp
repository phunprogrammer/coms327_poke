#include "PCTile.h"
#include "Screen.h"
#include <stdlib.h>

PCTile::PCTile(Screen& screen, coord_t coord) : 
    EntityTile(Entity::PC, coord, screen) {}

int PCTile::move() {
    coord_t move = { this->coord.x + this->direction.x, this->coord.y + this->direction.y };

    if(screen->getEntities()[move] != NULL_ENTITY_PTR && screen->getEntities()[move] != this) {
        setCoord(this->coord);
        return 0;
    }

    if(screen->getStructureMap()[move.y][move.x].getStructure() != NULL_STRUCT && speed.at(screen->getStructureMap()[move.y][move.x].getStructure()) == 0) {
        setCoord(this->coord);
        return 0;
    }

    if(speed.at(screen->getTerrainMap()[move.y][move.x].getTerrain()) == 0 && speed.at(screen->getStructureMap()[move.y][move.x].getStructure()) == 0) {
        setCoord(this->coord);
        return 0;
    }

    setCoord(move);
    screen->getPriority() += speed.at((*screen)[this->coord]);

    return 1;
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