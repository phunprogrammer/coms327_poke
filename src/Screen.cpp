#include "Screen.h"
#include "PerlinNoise.h"
#include <iostream>

Screen::Screen(waves_t waves, coord_t coord) : 
    coord(coord), 
    terrainMap(WIDTH, std::vector<TerrainTile>(LENGTH)), 
    structureMap(WIDTH, std::vector<StructureTile>(LENGTH)) {

    GenerateTerrain(waves);
    // GeneratePath(waves);
    // RandomizeBuildings();
    // DetectBorder();
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

// int Screen::GeneratePath(waves_t waves) {

// }
// int Screen::RandomizeBuildings() {

// }
// int Screen::DetectBorder() {

// }

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
    std::vector<std::vector<float>> expandedMap(WIDTH, std::vector<float>(LENGTH));

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
                expandedMap[y + WIDTH * (int)translation[i].y][x + LENGTH * (int)translation[i].x] = temp[y][x];
            }
        }
    }

    return expandedMap;
}

// int Screen::SelectEndpoints(float** altitudeMap, float** humidityMap) {

// }
// int Screen::GenerateBuildings() {

// }
// int Screen::isValidBuilding(int currX, int currY, int* value, int inverse, int vertical) {

// }
// int Screen::ConstructBuilding(building_t* building, StructureTile tile) {

// }

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