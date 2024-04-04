#include "AbstractTiles.h"

#ifndef PCTILE_H
#define PCTILE_H

class Screen;

class PCTile : public EntityTile {
    public:
        PCTile(Screen& screen, coord_t coord);
        ~PCTile() noexcept override = default;
        
        Entity getEntity() { return entity; }
        coord_t getCoord() { return coord; }
        void setCoord(coord_t coord);
        void setCoordRandom();
        void setScreen(Screen& screen) { this->screen = &screen; }
        int move();
};

#endif