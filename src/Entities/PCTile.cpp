#include "PCTile.h"
#include "Screen.h"
#include <stdlib.h>
#include <CursesHandler.h>
#include <Pokemon.h>

PCTile::PCTile(coord_t coord) : 
    EntityTile(Entity::PC, coord) {}

int PCTile::move() {
    coord_t move = { this->coord.x + this->direction.x, this->coord.y + this->direction.y };

    if(screen->getEntities()[move] != NULL_ENTITY_PTR && screen->getEntities()[move] != this && !((NPCTile*)screen->getEntities()[move])->isDefeated() && !this->isDefeated()) {
        setCoord(this->coord);
        int out = screen->getCursesHandler().BattleScreen((NPCTile*)screen->getEntities()[move], this);
        delete screen->getEntities()[move];
        return out;
    }

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
    screen->getCursesHandler().UpdateEntity(this);
    screen->getPriority() += speed.at((*screen)[this->coord]);

    if((*screen)[move] == Terrain::GRASSLAND && rand() % 100 <= 10 && this->party.size() > 0 && !this->isDefeated())
        screen->getCursesHandler().BattleScreen(this);

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

void PCTile::addToParty(Pokemon pokemon) {
    if(party.size() < 6)
        party.push_back(pokemon);
}

int PCTile::healParty() {
    for (auto& pokemon : party)
        pokemon.Heal();

    return 1;
}

int PCTile::isDefeated() {
    bool pcDefeated = true;
    for (auto& pokemon : party) {
        if (!pokemon.isFainted()) {
            pcDefeated = false;
            break;
        }
    }

    return pcDefeated;
}