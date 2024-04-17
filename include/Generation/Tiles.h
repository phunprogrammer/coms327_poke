#include "Config.h"
#include <map>

#ifndef TILES_H
#define TILES_H

enum Terrain : char {
    NULL_TERRAIN = ' ',
    FOREST = '^',
    MOUNTAIN = '%',
    CLEARING = '.',
    GRASSLAND = ':',
    OCEAN = '~'
};

const std::vector<Terrain> TERRAINS { 
    Terrain::FOREST, 
    Terrain::MOUNTAIN, 
    Terrain::CLEARING, 
    Terrain::GRASSLAND,
    Terrain::OCEAN
};

enum Structure : char {
    NULL_STRUCT = ' ',
    PATH = '#',
    PCNTR = 'C',
    PMART = 'M'
};

const std::map<char, int> WEIGHTS ({
    { Terrain::NULL_TERRAIN, 0 },
    { Terrain::FOREST, 50 },
    { Terrain::MOUNTAIN, 90 },
    { Terrain::CLEARING, 15 },
    { Terrain::GRASSLAND, 25 },
    { Terrain::OCEAN, 100 },
    { Structure::NULL_STRUCT, 0 },
    { Structure::PATH, -5 },
    { Structure::PCNTR, 100 },
    { Structure::PMART, 100 }
});

class TerrainTile {
    private:
        Terrain terrain;
        float height;
        float humidity;
        int weight;

    public:
        TerrainTile();
        TerrainTile(Terrain terrain, float height, float humidity);

        Terrain getTerrain() const { return terrain; }
        float getHeight() const { return height; }
        float getHumidity() const { return humidity; }
        int getWeight() const { return weight; }

        void setTerrain(Terrain terrain);
};

class StructureTile {
    private:
        Structure structure;
        int weight;

    public:
        StructureTile();
        StructureTile(Structure structure);

        Structure getStructure() const { return structure; }
        int getWeight() const { return weight; }

        void setStructure(Structure structure);
};

#endif