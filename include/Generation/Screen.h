#include "Config.h"
#include "EntityManager.h"
#include "AbstractTiles.h"
#include "MapVector.h"
#include "Tiles.h"
#include <queue>
#include <PQItem.h>
#include <CursesHandler.h>

#ifndef SCREEN_H
#define SCREEN_H

const std::vector<float> FREQUENCY({ 0.1f, 0.05f, 0.06f, 0.08f });
const std::vector<float> AMPLITUDE({ 1.0f, 1.5f, 1.0f, 0.8f });

const std::map<Terrain, double> MIN_HEIGHT ({
    { Terrain::FOREST, -0.5 },
    { Terrain::MOUNTAIN, 0.25 },
    { Terrain::CLEARING, -0.5 },
    { Terrain::GRASSLAND, -0.5 },
    { Terrain::OCEAN, -0.8 }
});

const std::map<Terrain, double> MIN_HUMIDITY ({
    { Terrain::FOREST, 0.35 },
    { Terrain::MOUNTAIN, -0.6 },
    { Terrain::CLEARING, -0.35 },
    { Terrain::GRASSLAND, 0.05 },
    { Terrain::OCEAN, -0.8 }
});

class EntityTile;
class EntityManager;
class PCTile;

class Screen {
    private:
        coord_t coord;
        std::vector<std::vector<TerrainTile>> terrainMap;
        std::vector<std::vector<StructureTile>> structureMap;

        int priority;
        MapVector<coord_t, EntityTile*> entities;
        EntityManager entityManager;
        CursesHandler cursesHandler;
        std::priority_queue<PQItem<EntityTile*>> moveQueue;
        gates_t gates;
        paths_t paths;

        int seed;

        void initialize(waves_t waves);
        int GenerateTerrain(waves_t waves);
        int GeneratePath(waves_t waves);
        int RandomizeBuildings();
        int DetectBorder();

        TerrainTile ChooseBiome(float altitude, float humidity);
        std::vector<std::vector<float>> GenerateNoiseMap(float scale, std::vector<wave_t> waves, coord_t offset);
        std::vector<std::vector<float>> ExpandWaveMap(std::vector<wave_t> waves);
        int SelectEndpoints(int vertical, std::vector<std::vector<float>> altitudeMap, std::vector<std::vector<float>> humidityMap);
        int GenerateBuildings();
        int isValidBuilding(int currX, int currY, int& value, int inverse, int vertical);
        int ConstructBuilding(building_t building, Structure tile);
    public:
        Screen(waves_t waves, coord_t coord, PCTile* player);
        Screen(waves_t waves, coord_t coord, PCTile* player, coord_t playerCoords);
        Screen(waves_t waves, coord_t coord, PCTile* player, MapVector<coord_t, EntityTile*> entities);
        Screen(waves_t waves, coord_t coord, PCTile* player, coord_t playerCoords, MapVector<coord_t, EntityTile*> entities);
        ~Screen();

        static waves_t GetWaves();
        static int RandScreen(float num);
        char operator[](const coord_t key) const;

        const std::vector<std::vector<TerrainTile>>& getTerrainMap() const { return terrainMap; }
        const std::vector<std::vector<StructureTile>>& getStructureMap() const { return structureMap; }
        std::priority_queue<PQItem<EntityTile*>>& getMoveQueue() { return moveQueue; }
        EntityManager& getEntityManager() { return entityManager; }
        CursesHandler& getCursesHandler() { return cursesHandler; }
        int& getPriority() { return priority; }
        int getSeed() const { return seed; }
        gates_t getGates() const { return gates; }
        paths_t getPaths() const { return paths; }
        coord_t getCoord() const { return coord; }

        void pushToQueue(const PQItem<EntityTile*>& item);

        MapVector<coord_t, EntityTile*>& getEntities() { return entities; }
};

#endif