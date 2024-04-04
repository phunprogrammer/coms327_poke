#include "AbstractTiles.h"

#ifndef WANDERERTILE_H
#define WANDERERTILE_H

class Screen;

class WandererTile : public NPCTile {
    public:
        WandererTile(Screen& screen);
        int move();
};

#endif