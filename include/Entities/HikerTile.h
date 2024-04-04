#include "AbstractTiles.h"

#ifndef HIKERTILE_H
#define HIKERTILE_H

class Screen;

class HikerTile : public NPCTile {
    public:
        HikerTile(Screen& screen);
        std::vector<path_t> move();
};

#endif