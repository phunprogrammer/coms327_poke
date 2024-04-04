#include "AbstractTiles.h"

#ifndef PACERTILE_H
#define PACERTILE_H

class Screen;

class PacerTile : public NPCTile {
    public:
        PacerTile(Screen& screen);
        int move();
};

#endif