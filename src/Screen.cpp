#include "Screen.h"
#include "PerlinNoise.h"
#include "AStar.h"
#include "PQItem.h"
#include "EntityManager.h"
#include "AbstractTiles.h"
#include "PCTile.h"
#include <iostream>
#include <queue>
#include <cmath>

Screen::Screen(waves_t waves, coord_t coord, PCTile* player) : 
    coord(coord), 
    terrainMap(WIDTH, std::vector<TerrainTile>(LENGTH)), 
    structureMap(WIDTH, std::vector<StructureTile>(LENGTH)),
    entities({{player->getCoord(), player}}), entityManager(*this) {

    initialize(waves);
    player->setScreen(*this);
    entityManager.RandomizePC();
}

Screen::Screen(waves_t waves, coord_t coord, PCTile* player, coord_t playerCoord) : 
    coord(coord), 
    terrainMap(WIDTH, std::vector<TerrainTile>(LENGTH)), 
    structureMap(WIDTH, std::vector<StructureTile>(LENGTH)),
    entities({{player->getCoord(), player}}), entityManager(*this) {
        
    initialize(waves);
    player->setScreen(*this);
    player->setCoord(playerCoord);
}

void Screen::initialize(waves_t waves) {
    GenerateTerrain(waves);
    GeneratePath(waves);
    RandomizeBuildings();
    DetectBorder();
}

int Screen::GenerateTerrain(waves_t waves) {
    coord_t tempCoord = { .x = coord.x * LENGTH, .y = coord.y * WIDTH };

    std::vector<std::vector<float>> heightMap = GenerateNoiseMap(1, waves.Height, tempCoord);
    std::vector<std::vector<float>> humidityMap = GenerateNoiseMap(1, waves.Humidity, tempCoord);

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            TerrainTile terrain = ChooseBiome(heightMap[y][x], humidityMap[y][x]);

            if ((x == 0 || y == 0 || y == WIDTH - 1 || x == LENGTH - 1) && 
                    terrain.getTerrain() != Terrain::FOREST && terrain.getTerrain() != Terrain::OCEAN) {
                terrain.setTerrain(Terrain::MOUNTAIN);
            }

            terrainMap[y][x] = terrain;
        }
    }

    return 1;
}

int Screen::GeneratePath(waves_t waves) {
    std::vector<std::vector<float>> expandedHeightMap = ExpandWaveMap(waves.Height);
    std::vector<std::vector<float>> expandedHumidityMap = ExpandWaveMap(waves.Humidity);

    SelectEndpoints(0, expandedHeightMap, expandedHumidityMap);

    for (int i = 0; i < PATHOFFSET; i++) {
        structureMap[gates.north.y + i][gates.north.x] = StructureTile(Structure::PATH);
        structureMap[gates.south.y - i][gates.south.x] = StructureTile(Structure::PATH);
    }

    SelectEndpoints(1, expandedHeightMap, expandedHumidityMap);

    for (int i = 0; i < PATHOFFSET; i++) {
        structureMap[gates.east.y][gates.east.x - i] = StructureTile(Structure::PATH);
        structureMap[gates.west.y][gates.west.x + i] = StructureTile(Structure::PATH);
    }

    int biomeGrid[WIDTH][LENGTH];

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = terrainMap[y][x].getWeight();
        }
    }

    paths.horizontalPath = aStar(biomeGrid, WIDTH - 4, LENGTH - 4, PATHOFFSET, gates.west.y, (LENGTH - 1) - PATHOFFSET, gates.east.y, 0.08, 4);

    for(path_t path : paths.horizontalPath)
        structureMap[path.coord.y][path.coord.x] = StructureTile(Structure::PATH);

    paths.verticalPath = aStar(biomeGrid, WIDTH - 4, LENGTH - 4, gates.north.x, PATHOFFSET, gates.south.x, (WIDTH - 1) - PATHOFFSET, 0.08, 4);

    for(path_t path : paths.verticalPath)
        structureMap[path.coord.y][path.coord.x] = StructureTile(Structure::PATH);

    return 1;
}

int Screen::RandomizeBuildings() {
    int chance = 100;
    int dist = abs((int)(coord.x) - MAXSIZE / 2) + abs((int)(coord.y) - MAXSIZE / 2);

    if (dist > 0)
        chance = std::pow(0.975, dist - 155) + 5;

    if((rand() % 100) < chance)
        GenerateBuildings();

    return 1;
}
int Screen::DetectBorder() {
    if (coord.x == MAXSIZE)
        structureMap[gates.east.y][gates.east.x].setStructure(Structure::NULL_STRUCT);
    if (coord.y == MAXSIZE)
        structureMap[gates.south.y][gates.south.x].setStructure(Structure::NULL_STRUCT);
    if (coord.x == MINSIZE)
        structureMap[gates.west.y][gates.west.x].setStructure(Structure::NULL_STRUCT);
    if (coord.y == MINSIZE)
        structureMap[gates.north.y][gates.north.x].setStructure(Structure::NULL_STRUCT);

    return 1;
}

TerrainTile Screen::ChooseBiome(float height, float humidity) {
    std::vector<Terrain> compatibleTerrain;

    for (const Terrain& terrain : TERRAINS)
        if (height >= MIN_HEIGHT.at(terrain) && humidity >= MIN_HUMIDITY.at(terrain))
            compatibleTerrain.push_back(terrain);

    if(compatibleTerrain.size() == 0)
        return TerrainTile();

    Terrain optimalTerrain = compatibleTerrain.at(0);
    float optimalTerrainValue = 10;

    for (const Terrain& terrain : compatibleTerrain) {
        float difference = (height - MIN_HEIGHT.at(terrain)) + (humidity - MIN_HUMIDITY.at(terrain));

        if(difference < optimalTerrainValue) {
            optimalTerrainValue = difference;
            optimalTerrain = terrain;
        }
    }

    return TerrainTile(optimalTerrain, height, humidity);
}

std::vector<std::vector<float>> Screen::GenerateNoiseMap(float scale, std::vector<wave_t> waves, coord_t offset) {
    std::vector<std::vector<float>> noiseMap(WIDTH, std::vector<float>(LENGTH));
    int x,y,k;

    //Reset 2D noise map
    for (y = 0; y < WIDTH; y++) {
        for (x = 0; x < LENGTH; x++) {
            noiseMap[y][x] = 0.0;
        }
    }

    //Assign a value to each float in the noise map
    for (y = 0; y < WIDTH; y++) {
        for (x = 0; x < LENGTH; x++) {
            //Get the x and y based on the scale and offset
            float posX = (float)x * scale + offset.x;
            float posY = (float)y * scale + offset.y;

            float normalization = 0.0f;

            //For each wave
            for (k = 0; k < (int)(waves.size()); k++) {
                noiseMap[y][x] += waves[k].amplitude * noise2(posX * waves[k].frequency + waves[k].seed, posY * waves[k].frequency + waves[k].seed);
                normalization += waves[k].amplitude;
            }

            noiseMap[y][x] /= normalization;
        }
    }

    return noiseMap;
}

std::vector<std::vector<float>> Screen::ExpandWaveMap(std::vector<wave_t> waves) {
    std::vector<std::vector<float>> expandedMap(WIDTH * 2, std::vector<float>(LENGTH * 2));

    coord_t pathOffsets[QUADRANT] = {
        [0] = { .x = LENGTH / -2 + coord.x * LENGTH, .y = WIDTH / -2 + coord.y * WIDTH },
        [1] =  { .x = (LENGTH - LENGTH / 2) + coord.x * LENGTH, .y = WIDTH / -2 + coord.y * WIDTH },
        [2] = { .x = LENGTH / -2 + coord.x * LENGTH, .y = (WIDTH - WIDTH / 2) + coord.y * WIDTH },
        [3] = { .x = (LENGTH - LENGTH / 2) + coord.x * LENGTH, .y = (WIDTH - WIDTH / 2) + coord.y * WIDTH }
    };

    coord_t translation[QUADRANT] = {
        [0] = { .x = 0, .y = 0 },
        [1] =  { .x = 1, .y = 0 },
        [2] = { .x = 0, .y = 1 },
        [3] = { .x = 1, .y = 1 }
    };

    for(int i = 0; i < QUADRANT; i++) {
        std::vector<std::vector<float>> temp = GenerateNoiseMap(1, waves, pathOffsets[i]);

        for(int y = 0; y < WIDTH; y++) {
            for(int x = 0; x < LENGTH; x++) {
                expandedMap[y + WIDTH * translation[i].y][x + LENGTH * translation[i].x] = temp[y][x];
            }
        }
    }

    return expandedMap;
}

int Screen::SelectEndpoints(int horizontal, std::vector<std::vector<float>> heightMap, std::vector<std::vector<float>> humidityMap) {
    int width = WIDTH;
    int length = LENGTH;

    if(horizontal) {
        width = LENGTH;
        length = WIDTH;
    }

    for(int i = 0; i < 2; i++) {
        float bestAbsAvg = length * WEIGHTS.at(Terrain::OCEAN) * 2;
        for(int x = length / 2 + PATHOFFSET; x < length + length / 2 - PATHOFFSET; x++) {
            int average = 0;
            for(int y = width / 2 - PATHMARGIN + (width * i); y < width / 2 + PATHMARGIN + (width * i); y++) {
                int j = y;
                int k = x;

                if(horizontal) {
                    j = x;
                    k = y;
                }

                average += ChooseBiome(heightMap[j][k], humidityMap[j][k]).getWeight();
            }

            average /= PATHMARGIN * 2;

            float absAvg = average * length + (abs(length - x) / 4);

            if(absAvg < bestAbsAvg) {
                bestAbsAvg = absAvg;

                if(horizontal) {
                    if(i == 0) {
                        gates.west.y = x - length / 2;
                        gates.west.x = 0;
                    }
                    else {
                        gates.east.y = x - length / 2;
                        gates.east.x = LENGTH - 1;
                    }
                }
                else {
                    if(i == 0) {
                        gates.north.x = x - length / 2;
                        gates.north.y = 0;
                    }
                    else {
                        gates.south.x = x - length / 2;
                        gates.south.y = WIDTH - 1;
                    }
                }
            }
        }
    }

    return 1;
}

int Screen::GenerateBuildings() {
    int horizontalBias = rand() % LENGTH;
    int verticalBias = rand() % WIDTH;

    std::priority_queue<PQItem<building_t>> buildingQueue;

    for(path_t path : paths.horizontalPath) {
        int x = path.coord.x;
        int y = path.coord.y;

        for(int i = -1; i <= 1; i += 2) {
            int value = 0;

            if (isValidBuilding(x, y, value, i, 0)) {
                building_t building;
                building.path = path;
                building.inverse = i;
                building.vertical = 0;
                buildingQueue.push(PQItem(building, value + abs(horizontalBias - x) + rand() % 10));
            }
        }
    }

    for(path_t path : paths.verticalPath) {
        int x = path.coord.x;
        int y = path.coord.y;

        for(int i = -1; i < 0; i += 2) {
            int value = 0;

            if (isValidBuilding(x, y, value, i, 1)) {
                building_t building;
                building.path = path;
                building.inverse = i;
                building.vertical = 1;
                buildingQueue.push(PQItem(building, value + abs(verticalBias - y) + rand() % 10));
            }
        }
    }

    if(buildingQueue.empty())
        return 0;

    building_t pokemart = buildingQueue.top().getData();
    buildingQueue.pop();
    ConstructBuilding(pokemart, Structure::PMART); 

    int constructed = 1;

    while(constructed == 1 && !buildingQueue.empty()){
        building_t pokecenter = buildingQueue.top().getData();
        buildingQueue.pop();
        constructed = ConstructBuilding(pokecenter, Structure::PCNTR);
    }

    return 1;
}

int Screen::isValidBuilding(int currX, int currY, int &value, int inverse, int vertical) {
    coord_t buildingOffset[QUADRANT] = {
        [0] = { .x = 0, .y = 1 },
        [1] =  { .x = 0, .y = 2 },
        [2] = { .x = 1, .y = 2 },
        [3] = { .x = 1, .y = 1 }
    };

    for (int j = 0; j < QUADRANT; j++) {
        int newX = 0;
        int newY = 0;

        if (!vertical) {
            newX = currX + buildingOffset[j].x;
            newY = currY + buildingOffset[j].y * inverse;
        }
        else {
            newX = currX + buildingOffset[j].y * inverse;
            newY = currY + buildingOffset[j].x;
        }

        if((newX <= 0 && newY && 0 && newX >= LENGTH - 1 && newY >= WIDTH - 1) || structureMap[newY][newX].getStructure() == Structure::PATH)
            return 0;

        value += terrainMap[newY][newX].getWeight();
    }

    if(!vertical && structureMap[currY][currX + 1].getStructure() != Structure::PATH)
        return 0;

    if(vertical && structureMap[currY + 1][currX].getStructure() != Structure::PATH)
        return 0;

    value /= QUADRANT;

    return 1;
}

int Screen::ConstructBuilding(building_t building, Structure tile) {
    coord_t buildingOffset[QUADRANT] = {
        [0] = { .x = 0, .y = 1 },
        [1] =  { .x = 0, .y = 2 },
        [2] = { .x = 1, .y = 2 },
        [3] = { .x = 1, .y = 1 }
    };

    coord_t placed[QUADRANT];

    int currX = building.path.coord.x;
    int currY = building.path.coord.y;
    int inverse = building.inverse;
    int vertical = building.vertical;

    for (int j = 0; j < QUADRANT; j++) {
        int newX = 0;
        int newY = 0;

        if (!vertical) {
            newX = currX + buildingOffset[j].x;
            newY = currY + buildingOffset[j].y * inverse;
        }
        else {
            newX = currX + buildingOffset[j].y * inverse;
            newY = currY + buildingOffset[j].x;
        }

        if (structureMap[newY][newX].getStructure() == Structure::PMART)
            return 1;

        
        placed[j].x = newX;
        placed[j].y = newY;
    }

    for (int j = 0; j < QUADRANT; j++) {
        structureMap[(int)(placed[j].y)][(int)(placed[j].x)] = StructureTile(tile);
    }

    return 0;
}

waves_t Screen::GetWaves(int* seed) {
    waves_t waves;

    int i = 0;
    *seed = rand();

    for (; i < WAVENUM; i++) {
        wave_t wave;
        wave.amplitude = AMPLITUDE[i];
        wave.frequency = FREQUENCY[i];
        wave.seed = *seed / 10000.0;
        waves.Height.push_back(wave);
    }

    for (int j = 0; j < WAVENUM; j++) {
        wave_t wave;
        wave.amplitude = AMPLITUDE[i];
        wave.frequency = FREQUENCY[i];
        wave.seed = *seed / 10000.0;
        waves.Humidity.push_back(wave);
    }

    return waves;
}