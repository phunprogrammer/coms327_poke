#include "AbstractTiles.h"

#ifndef WANDERERTILE_H
#define WANDERERTILE_H

class Screen;

class WandererTile : public NPCTile {
    private:
        coord_t direction;
    public:
        WandererTile(Screen& screen);
        std::vector<path_t> move();
};

#endif