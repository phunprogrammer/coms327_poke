#include "Config.h"
#include "Tiles.h"

#include <map>

#ifndef ENTITIES_H
#define ENTITIES_H

enum Entity : char {
    NULL_ENTITY = ' ',
    PC = '@',
    HIKER = 'h',
    RIVAL = 'r',
    PACER = 'p',
    WANDERER = 'w',
    SENTRY = 's',
    EXPLORER = 'e'
};

class Screen;

class EntityTile {
    protected:
        EntityTile(Entity entity, coord_t coord, Screen& screen, std::map<char, int> speed);
        Entity entity;
        coord_t coord;
        Screen* screen;
        std::map<char, int> speed;
    public:
        virtual Entity getEntity() = 0;
        virtual coord_t getCoord() = 0;
        virtual void setCoord(coord_t coord) = 0;
};

const std::map<char, int> PC_SPEED ({
    { Terrain::NULL_TERRAIN, 0 },
    { Terrain::FOREST, 0 },
    { Terrain::MOUNTAIN, 0 },
    { Terrain::CLEARING, 10 },
    { Terrain::GRASSLAND, 20 },
    { Terrain::OCEAN, 0 },
    { Structure::NULL_STRUCT, 0 },
    { Structure::PATH, 10 },
    { Structure::PMART, 10 },
    { Structure::PCNTR, 10 }
});

class PCTile : public EntityTile {
    public:
        PCTile(Screen& screen, coord_t coord);
        
        Entity getEntity() { return entity; }
        coord_t getCoord() { return coord; }
        void setCoord(coord_t coord) { this->coord = coord; }
        void setScreen(Screen& screen) { this->screen = &screen; }
};

// class NPCTile : public EntityTile {
//     protected:
//         bool defeated;
//     public:
//         virtual std::vector<path_t> move() = 0;
// };

// class HikerTile : public NPCTile {
//     public:
//         HikerTile(Screen& screen);
//         std::vector<path_t> move();
// };


#endif