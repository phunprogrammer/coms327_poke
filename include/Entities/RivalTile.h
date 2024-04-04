#include "AbstractTiles.h"

#ifndef RivalTile_H
#define RivalTile_H

class Screen;

class RivalTile : public NPCTile {
    public:
        RivalTile(Screen& screen);
        std::vector<path_t> move();
};

#endif