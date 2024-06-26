#include "Config.h"
#include "AbstractTiles.h"
#include "Screen.h"
#include <stdlib.h>
#include "PCTile.h"

EntityTile::EntityTile(Entity entity, coord_t coord, Screen& screen) : 
    entity(entity), screen(&screen), speed(ENTITY_SPEED.at(entity)), coord(coord), direction(randomDirection()) {}

EntityTile::EntityTile(Entity entity, Screen& screen) : 
    entity(entity), screen(&screen), speed(ENTITY_SPEED.at(entity)), coord(randomCoord()),  direction(randomDirection()) {}

EntityTile::EntityTile(Entity entity, coord_t coord) : 
    entity(entity), speed(ENTITY_SPEED.at(entity)), coord(coord), direction(randomDirection()) {}

coord_t EntityTile::randomCoord() {
    coord_t coord;
    do {
        coord.x = rand() % (LENGTH - 2) + 1;
        coord.y = rand() % (WIDTH - 2) + 1;
    } while(speed.at(screen->getTerrainMap()[coord.y][coord.x].getTerrain()) == 0 || 
                (screen->getStructureMap()[coord.y][coord.x].getStructure() != Structure::PATH && 
                screen->getStructureMap()[coord.y][coord.x].getStructure() != Structure::NULL_STRUCT) ||
                screen->getEntities()[{ coord.x, coord.y }] != NULL_ENTITY_PTR);
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
        GenerateParty();
    }

NPCTile::NPCTile(Entity entity, coord_t coord, Screen& screen) :
    EntityTile(entity, coord, screen), defeated(0) {
        screen.getEntities().insert(coord, this);
        GenerateParty();
    }

void NPCTile::setCoord(coord_t coord) {
    this->prevCoord = this->coord;
    screen->getEntities().move(this->coord, coord);
    this->coord = coord;
}

int NPCTile::ValidMove(coord_t move) {
    if(move.x <= 0 || move.x >= LENGTH - 1 || move.y <= 0 || move.y >= WIDTH - 1)
        return 0;

    if(screen->getEntities()[move] != NULL_ENTITY_PTR)
        return 0;

    if(screen->getStructureMap()[move.y][move.x].getStructure() != NULL_STRUCT && speed.at(screen->getStructureMap()[move.y][move.x].getStructure()) == 0)
        return 0;

    if(speed.at(screen->getTerrainMap()[move.y][move.x].getTerrain()) == 0 && speed.at(screen->getStructureMap()[move.y][move.x].getStructure()) == 0)
        return 0;

    return 1;
}

int NPCTile::move() {
    coord_t move = { this->coord.x + this->direction.x, this->coord.y + this->direction.y };

    if(screen->getEntities()[move] != NULL_ENTITY_PTR && screen->getEntities()[move]->getEntity() == Entity::PC && !this->defeated && !((PCTile*)screen->getEntities()[move])->isDefeated()) {
        screen->getCursesHandler().BattleScreen(this, (PCTile*)screen->getEntities()[move]);
    }

    if(!ValidMove(move)) {
        setCoord(this->coord);
        queueMove();
        return 0;
    }

    setCoord(move);
    screen->getCursesHandler().UpdateEntity(this);
    queueMove();

    return 1;
}

void NPCTile::GenerateParty() {
    party.push_back(Pokemon(screen->getCoord()));

    while(rand() % 100 < 60 && party.size() < 6)
        party.push_back(Pokemon(screen->getCoord()));
}