#include "Tiles.h"

TerrainTile::TerrainTile(Terrain terrain, float height, float humidity) : 
            terrain(terrain), height(height), humidity(humidity), weight(WEIGHTS.at(terrain)) {}

TerrainTile::TerrainTile() : 
            terrain(Terrain::NULL_TERRAIN), height(0), humidity(0), weight(0) {}

void TerrainTile::setTerrain(Terrain terrain) {
    this->terrain = terrain;
    this->weight = WEIGHTS.at(terrain);
}

StructureTile::StructureTile(Structure structure) : 
            structure(structure), weight(WEIGHTS.at(structure)) {}

StructureTile::StructureTile() : 
            structure(Structure::NULL_STRUCT), weight(0) {}

void StructureTile::setStructure(Structure structure) {
    this->structure = structure;
    this->weight = WEIGHTS.at(structure);
}