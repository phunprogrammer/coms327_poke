#include "AbstractTiles.h"

#ifndef ExplorerTile_H
#define ExplorerTile_H

class Screen;

class ExplorerTile : public NPCTile {
    private:
        coord_t direction;
    public:
        ExplorerTile(Screen& screen);
        std::vector<path_t> move();
};

#endif