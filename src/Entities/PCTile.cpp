#include "PCTile.h"
#include "Screen.h"

PCTile::PCTile(Screen& screen, coord_t coord) : 
    EntityTile(Entity::PC, coord, screen) {}