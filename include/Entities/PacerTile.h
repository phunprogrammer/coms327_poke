#include "AbstractTiles.h"

#ifndef PACERTILE_H
#define PACERTILE_H

class Screen;

class PacerTile : public NPCTile {
    private:
        coord_t direction;
    public:
        PacerTile(Screen& screen);
        std::vector<path_t> move();
};

#endif