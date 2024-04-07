#include "Config.h"
#include "AbstractTiles.h"
#include "Screen.h"
#include <stdlib.h>

EntityTile::EntityTile(Entity entity, coord_t coord, Screen& screen) : 
    entity(entity), screen(&screen), speed(ENTITY_SPEED.at(entity)), coord(coord), direction(randomDirection()) {}

EntityTile::EntityTile(Entity entity, Screen& screen) : 
    entity(entity), screen(&screen), speed(ENTITY_SPEED.at(entity)), coord(randomCoord()),  direction(randomDirection()) {}

coord_t EntityTile::randomCoord() {
    coord_t coord;
    do {
        coord.x = rand() % (LENGTH - 2) + 1;
        coord.y = rand() % (WIDTH - 2) + 1;
    } while(speed.at(screen->getTerrainMap()[coord.y][coord.x].getTerrain()) == 0 || 
                (screen->getStructureMap()[coord.y][coord.x].getStructure() != Structure::PATH && 
                screen->getStructureMap()[coord.y][coord.x].getStructure() != Structure::NULL_STRUCT) ||
                screen->getEntities()[{ coord.y, coord.x }] != NULL_ENTITY_PTR);
    return coord;
}

coord_t EntityTile::randomDirection() {
    coord_t direction;
    direction.x = (rand() % 3) - 1;
    direction.y = (direction.x == 0) ? ((rand() % 2 == 0) ? -1 : 1) : (rand() % 3) - 1;
    return direction;
}

NPCTile::NPCTile(Entity entity, Screen& screen) :
    EntityTile(entity, screen), defeated(0) {
        screen.getEntities().insert(coord, this);
    }

NPCTile::NPCTile(Entity entity, coord_t coord, Screen& screen) :
    EntityTile(entity, coord, screen), defeated(0) {
        screen.getEntities().insert(coord, this);
    }

void NPCTile::setCoord(coord_t coord) {
    this->prevCoord = this->coord;
    screen->getEntities().move(this->coord, coord);
    this->coord = coord;
}