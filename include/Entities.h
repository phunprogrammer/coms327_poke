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
        Screen& screen;
        std::map<char, int> speed;
    public:
        virtual Entity getEntity();
        virtual int Spawn();
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
        int Spawn();

        Entity getEntity() { return entity; }
};

class NPCTile : public EntityTile {
    protected:
        bool defeated;
    public:
        virtual std::vector<path_t> move() = 0;
};

class HikerTile : public NPCTile {
    public:
        HikerTile(Screen& screen);
        std::vector<path_t> move();
};


#endif