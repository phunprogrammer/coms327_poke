#include "AbstractTiles.h"

#ifndef SENTRYTILE_H
#define SENTRYTILE_H

class Screen;

class SentryTile : public NPCTile {
    public:
        SentryTile(Screen& screen);
        std::vector<path_t> move();
};

#endif