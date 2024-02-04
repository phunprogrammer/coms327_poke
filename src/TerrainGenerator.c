#include "NoiseGenerator.h"
#include "Config.h"
#include "TerrainGenerator.h"
#include "PPMGenerator.h"
#include "AStar.h"
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

static vector_t offset = { .x = 0, .y = 0 };

screen_t ScreenGenerator(waves_t waves) {
    screen_t screen = { .screenCoords = offset };

    GenerateTerrain(waves, &screen);
    GeneratePath(waves, &screen);
    //printf("(%d, %d) = Start: %d, End: %d\n", screen.screenCoords.x, screen.screenCoords.y, screen.horizontalPath.start, screen.horizontalPath.end);

    return screen;
}

tileType_t** GenerateTerrain(waves_t waves, screen_t* screen) {
    screen->biomeMap = (tileType_t**)malloc(WIDTH * sizeof(tileType_t*));

    for (int i = 0; i < WIDTH; ++i)
        screen->biomeMap[i] = (tileType_t*)malloc(LENGTH * sizeof(tileType_t));

    noisemap_t altitudeMap = Generate(1, waves.Altitude, offset);
    noisemap_t humidityMap = Generate(1, waves.Humidity, offset);

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            tileType_t biome = ChooseBiome(altitudeMap.map[y][x], humidityMap.map[y][x]);

            if ((x == 0 || y == 0 || y == WIDTH - 1 || x == LENGTH - 1) && biome.biomeID != FOREST) {
                biome = Tiles[MOUNTAIN];
            }

            biome.minHeight = altitudeMap.map[y][x];
            biome.minHumidity = humidityMap.map[y][x];

            screen->biomeMap[y][x] = biome;
        }
    }

    return screen->biomeMap;
}

void FreeBiomeArray(tileType_t** array, int width) {
    for (int i = 0; i < width; ++i) {
        free(array[i]);
    }
    
    free(array);
}

tileType_t ChooseBiome(float altitude, float humidity) {
    tileType_t compatibleBiomes[BIOMENUM];
    int numCompatible = 0;

    for (int i = 0; i < BIOMENUM; i++) {
        if (altitude >= Tiles[i].minHeight && humidity >= Tiles[i].minHumidity) {
            compatibleBiomes[numCompatible] = Tiles[i];
            numCompatible++;
        }
    }

    if (numCompatible == 0)
        return Tiles[OCEAN];
    
    int bestBiomeIndex = 0;
    float bestBiomeVal = 10.0;

    for (int i = 0; i < numCompatible; i++) {
        float difference = (altitude - compatibleBiomes[i].minHeight) + (humidity - compatibleBiomes[i].minHumidity);

        if (difference < bestBiomeVal) {
            bestBiomeVal = difference;
            bestBiomeIndex = i;
        }
    }

    return compatibleBiomes[bestBiomeIndex];
}

void UpdateOffset(int x, int y) {
    offset.x = x * LENGTH;
    offset.y = y * WIDTH;
}

void GeneratePath(waves_t waves, screen_t* screen) {
    expandedmap_t expandedAltitudeMap = MapExpander(waves.Altitude);
    expandedmap_t expandedHumidityMap = MapExpander(waves.Humidity);

    endPointSelector(&(screen->verticalPath), WIDTH, LENGTH, expandedAltitudeMap, expandedHumidityMap);

    for (int i = 0; i < PATHOFFSET; i++) {
        SwitchTile(&(screen->biomeMap[i][screen->verticalPath.start]), Tiles[PATH]);
        SwitchTile(&(screen->biomeMap[(WIDTH - 1) - i][screen->verticalPath.end]), Tiles[PATH]);
    }

    endPointSelector(&(screen->horizontalPath), LENGTH, WIDTH, expandedAltitudeMap, expandedHumidityMap);

    for (int i = 0; i < PATHOFFSET; i++) {
        SwitchTile(&(screen->biomeMap[screen->horizontalPath.start][i]), Tiles[PATH]);
        SwitchTile(&(screen->biomeMap[screen->horizontalPath.end][(LENGTH - 1)- i]), Tiles[PATH]);
    }


    float** biomeGrid = (float**)malloc(WIDTH * sizeof(float*));

    for (int y = 0; y < WIDTH; y++) {
        biomeGrid[y] = (float*)malloc(LENGTH * sizeof(float));

        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = screen->biomeMap[y][x].weight;
        }
    }

    asnode_t* horizontalPath = aStar(biomeGrid, WIDTH, LENGTH, PATHOFFSET, screen->horizontalPath.start, (LENGTH - 1) - PATHOFFSET, screen->horizontalPath.end);

    while(horizontalPath != NULL) {
        int x = horizontalPath->x;
        int y = horizontalPath->y;

        SwitchTile(&(screen->biomeMap[y][x]), Tiles[PATH]);

        horizontalPath = horizontalPath->previous;
    }

    
    for (int y = 0; y < WIDTH; y++) {
        biomeGrid[y] = (float*)malloc(LENGTH * sizeof(float));

        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = screen->biomeMap[y][x].weight;
        }
    }

    asnode_t* verticalPath = aStar(biomeGrid, WIDTH, LENGTH, screen->verticalPath.start, PATHOFFSET, screen->verticalPath.end, (WIDTH - 1) - PATHOFFSET);

    for (int i = 0; i < WIDTH; ++i) {
        free(biomeGrid[i]);
    }
    
    free(biomeGrid);

    while(verticalPath != NULL) {
        int x = verticalPath->x;
        int y = verticalPath->y;

        SwitchTile(&(screen->biomeMap[y][x]), Tiles[PATH]);

        verticalPath = verticalPath->previous;
    }
}

void SwitchTile (tileType_t* tileA, tileType_t tileB) {
    float tempAltitude = tileA->minHeight;
    float tempHumidity = tileA->minHumidity;
    *tileA = tileB;
    tileA->minHeight = tempAltitude;
    tileA->minHumidity = tempHumidity;
}

int endPointSelector(path_t* path, int width, int length, expandedmap_t altitudeMap, expandedmap_t humidityMap) {
    path->start = 0;
    path->end = 0;

    for(int i = 0; i < 2; i++) {
        float bestAbsAvg = length * 100;
        for(int x = length / 2 + PATHOFFSET; x < length + length / 2 - PATHOFFSET; x++) {
            int average = 0;
            for(int y = width / 2 - PATHMARGIN + (width * i); y < width / 2 + PATHMARGIN + (width * i); y++) {
                int j = y;
                int k = x;

                if(length == WIDTH) {
                    j = x;
                    k = y;
                }

                average += ChooseBiome(altitudeMap.map[j][k], humidityMap.map[j][k]).weight;
            }

            average /= PATHMARGIN * 2;

            float absAvg = average * length + (abs(length - x) / 4);

            if(absAvg < bestAbsAvg) {
                bestAbsAvg = absAvg;

                if(i == 0) path->start = x - length / 2;
                else path->end = x - length / 2;
            }
        }
    }

    return 0;
}

expandedmap_t MapExpander (wave_t wave[WAVENUM]) {
    expandedmap_t expandedMap;

    vector_t pathOffsets[QUADRANT] = {
        [0] = { .x = LENGTH / -2 + offset.x, .y = WIDTH / -2 + offset.y },
        [1] =  { .x = (LENGTH - LENGTH / 2) + offset.x, .y = WIDTH / -2 + offset.y },
        [2] = { .x = LENGTH / -2 + offset.x, .y = (WIDTH - WIDTH / 2) + offset.y },
        [3] = { .x = (LENGTH - LENGTH / 2) + offset.x, .y = (WIDTH - WIDTH / 2) + offset.y }
    };

    vector_t translation[QUADRANT] = {
        [0] = { .x = 0, .y = 0 },
        [1] =  { .x = 1, .y = 0 },
        [2] = { .x = 0, .y = 1 },
        [3] = { .x = 1, .y = 1 }
    };

    for(int i = 0; i < QUADRANT; i++) {
        noisemap_t temp = Generate(1, wave, pathOffsets[i]);

        for(int y = 0; y < WIDTH; y++) {
            for(int x = 0; x < LENGTH; x++) {
                expandedMap.map[y + WIDTH * (int)translation[i].y][x + LENGTH * (int)translation[i].x] = temp.map[y][x];
            }
        }
    }

    return expandedMap;
}