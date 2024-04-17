#include "AbstractTiles.h"

#ifndef PCTILE_H
#define PCTILE_H

class Screen;
class Pokemon;

class PCTile : public EntityTile {
    public:
        PCTile(coord_t coord);
        ~PCTile() noexcept override = default;
        
        Entity getEntity() { return entity; }
        coord_t getCoord() { return coord; }
        coord_t getPrevCoord() { return prevCoord; }
        coord_t getDirection() { return direction; }
        void setScreen(Screen* screen) { this->screen = screen; }
        void setCoord(coord_t coord);
        void setDirection(coord_t coord) { this->direction = coord; }
        void setCoordRandom();
        void setScreen(Screen& screen) { this->screen = &screen; }
        int move();
        void addToParty(Pokemon pokemon);
        std::vector<Pokemon>& getParty() { return party; }
        int healParty();
        int isDefeated();
};

#endif