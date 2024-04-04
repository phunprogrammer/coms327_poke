#include "Config.h"
#include "Entities.h"
#include "Screen.h"

EntityTile::EntityTile(Entity entity, coord_t coord, Screen& screen, std::map<char, int> speed) : 
    entity(entity), coord(coord), screen(&screen), speed(speed) {}

PCTile::PCTile(Screen& screen, coord_t coord) : 
    EntityTile(Entity::PC, coord, screen, PC_SPEED) {}