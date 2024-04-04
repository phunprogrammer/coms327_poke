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
        EntityTile(Entity entity, coord_t coord, Screen& screen);
        EntityTile(Entity entity, Screen& screen);
        Entity entity;
        coord_t coord;
        Screen* screen;
        std::map<char, int> speed;
    public:
        virtual Entity getEntity() = 0;
        virtual coord_t getCoord() = 0;
        virtual void setCoord(coord_t coord) = 0;
};

class NPCTile : public EntityTile {
    protected:
        NPCTile(Entity entity, Screen& screen);
        coord_t random();
        coord_t randomDirection();
        bool defeated;
    public:
        virtual std::vector<path_t> move() = 0;
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

const std::map<char, int> HIKER_SPEED ({
    { Terrain::NULL_TERRAIN, 0 },
    { Terrain::FOREST, 15 },
    { Terrain::MOUNTAIN, 15 },
    { Terrain::CLEARING, 10 },
    { Terrain::GRASSLAND, 15 },
    { Terrain::OCEAN, 0 },
    { Structure::NULL_STRUCT, 0 },
    { Structure::PATH, 10 },
    { Structure::PMART, 0 },
    { Structure::PCNTR, 0 }
});

const std::map<char, int> DEFAULT_SPEED ({
    { Terrain::NULL_TERRAIN, 0 },
    { Terrain::FOREST, 0 },
    { Terrain::MOUNTAIN, 0 },
    { Terrain::CLEARING, 10 },
    { Terrain::GRASSLAND, 20 },
    { Terrain::OCEAN, 0 },
    { Structure::NULL_STRUCT, 0 },
    { Structure::PATH, 10 },
    { Structure::PMART, 0 },
    { Structure::PCNTR, 0 }
});

const std::map<Entity, std::map<char, int>> ENTITY_SPEED ({
    { Entity::PC, PC_SPEED },
    { Entity::HIKER, HIKER_SPEED },
    { Entity::RIVAL, DEFAULT_SPEED },
    { Entity::PACER, DEFAULT_SPEED },
    { Entity::WANDERER, DEFAULT_SPEED },
    { Entity::EXPLORER, DEFAULT_SPEED }
});

#endif