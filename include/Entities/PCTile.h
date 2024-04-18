#include "AbstractTiles.h"
#include "Items.h"

#ifndef PCTILE_H
#define PCTILE_H

class Screen;
class Pokemon;

class PCTile : public EntityTile {
    private:
        std::map<ItemEnum, int> bag;
    public:
        PCTile(coord_t coord);
        ~PCTile();
        
        Entity getEntity() { return entity; }
        coord_t getCoord() { return coord; }
        coord_t getPrevCoord() { return prevCoord; }
        coord_t getDirection() { return direction; }
        std::map<ItemEnum, int>& getBag() { return bag; }
        void setScreen(Screen* screen) { this->screen = screen; }
        void setCoord(coord_t coord);
        void setDirection(coord_t coord) { this->direction = coord; }
        void setCoordRandom();
        void setScreen(Screen& screen) { this->screen = &screen; }
        int move();
        int addToParty(Pokemon pokemon);
        int addToBag(ItemEnum item);
        int removeFromBag(ItemEnum item);
        std::vector<Pokemon>& getParty() { return party; }
        int healParty();
        int isDefeated();
};

#endif