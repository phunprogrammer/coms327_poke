#include "Config.h"
#include "Tiles.h"

#include <map>

#ifndef ABSTRACTTILES_H
#define ABSTRACTTILES_H

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

const std::vector<Entity> ENTITIES({ HIKER, RIVAL, PACER, WANDERER, SENTRY, EXPLORER });

class Screen;

class EntityTile {
    protected:
        EntityTile(Entity entity, coord_t coord, Screen& screen);
        EntityTile(Entity entity, Screen& screen);
        EntityTile(Entity entity, coord_t coord);
        coord_t randomDirection();
        coord_t randomCoord();
        
        Entity entity;
        Screen* screen;
        std::map<char, int> speed;
        coord_t coord;
        coord_t prevCoord;
        coord_t direction;
    public:
        virtual Entity getEntity() = 0;
        virtual coord_t getCoord() = 0;
        virtual coord_t getPrevCoord() = 0;
        virtual void setCoord(coord_t coord) = 0;
        virtual int move() = 0;
        virtual ~EntityTile() noexcept = default;
};

class NPCTile : public EntityTile {
    protected:
        NPCTile(Entity entity, coord_t coord, Screen& screen);
        NPCTile(Entity entity, Screen& screen);
        int ValidMove(coord_t move);
        bool defeated;
    public:
        ~NPCTile() noexcept override = default;
        Entity getEntity() { return entity; }
        coord_t getCoord() { return coord; }
        coord_t getPrevCoord() { return prevCoord; }
        bool isDefeated() { return defeated; }
        void defeat() { defeated = true; }
        void setCoord(coord_t coord);
        int move();
        virtual int queueMove() = 0;
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
    { Entity::EXPLORER, DEFAULT_SPEED },
    { Entity::SENTRY, DEFAULT_SPEED }
});

#endif