#include "AbstractTiles.h"

#ifndef RIVALTILE_H
#define RIVALTILE_H

class Screen;

class RivalTile : public NPCTile {
    public:
        RivalTile(Screen& screen);
        std::vector<path_t> move();
};

#endif