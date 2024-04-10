#include "AbstractTiles.h"

#ifndef EXPLORERTILE_H
#define EXPLORERTILE_H

class Screen;

class ExplorerTile : public NPCTile {
    public:
        ExplorerTile(Screen& screen);
        int queueMove();
};

#endif