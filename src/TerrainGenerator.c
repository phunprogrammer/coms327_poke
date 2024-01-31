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

biomeType_t** GenerateTerrain(waves_t waves, screen_t* screen) {
    screen->biomeMap = (biomeType_t**)malloc(WIDTH * sizeof(biomeType_t*));

    for (int i = 0; i < WIDTH; ++i)
        screen->biomeMap[i] = (biomeType_t*)malloc(LENGTH * sizeof(biomeType_t));

    noisemap_t altitudeMap = Generate(1, waves.Altitude, offset);
    noisemap_t humidityMap = Generate(1, waves.Humidity, offset);
    float maxScore = 0;

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            biomeType_t biome = { .minHeight = altitudeMap.map[y][x], .minHumidity = humidityMap.map[y][x] };
            float tempScore = OptimalScore(altitudeMap.map[y][x], humidityMap.map[y][x]);

            if (tempScore > maxScore) {
                maxScore = tempScore;
            }

            if (x == 0 || y == 0 || y == WIDTH - 1 || x == LENGTH - 1) {
                biome.biomeID = Biomes[MOUNTAIN].biomeID;
                biome.type = Biomes[MOUNTAIN].type;
            }
            else {
                biomeType_t tempBiome = ChooseBiome(altitudeMap.map[y][x], humidityMap.map[y][x]);
                biome.biomeID = tempBiome.biomeID;
                biome.type = tempBiome.type;
            }

            screen->biomeMap[y][x] = biome;
        }
    }

    return screen->biomeMap;
}

void FreeBiomeArray(biomeType_t** array, int width, int length) {
    for (int i = 0; i < width; ++i) {
        free(array[i]);
    }
    
    free(array);
}

biomeType_t ChooseBiome(float altitude, float humidity) {
    biomeType_t compatibleBiomes[BIOMENUM];
    int numCompatible = 0;

    for (int i = 0; i < BIOMENUM; i++) {
        if (altitude >= Biomes[i].minHeight && humidity >= Biomes[i].minHumidity) {
            compatibleBiomes[numCompatible] = Biomes[i];
            numCompatible++;
        }
    }

    if (compatibleBiomes == 0)
        return Biomes[OCEAN];
    
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

    screen->verticalPath.start = 0;
    screen->verticalPath.end = 0;

    for(int i = 0; i < 2; i++) {
        float bestAbsAvg = 100;
        for(int x = LENGTH / 2 + 1; x < LENGTH + LENGTH / 2 - 1; x++) {
            float averageAltitude = 0;
            float averageHumdity = 0;
            for(int y = WIDTH / 2 - PATHMARGIN + (WIDTH * i); y < WIDTH / 2 + PATHMARGIN + (WIDTH * i); y++) {
                averageAltitude += expandedAltitudeMap.map[y][x];
                averageHumdity += expandedHumidityMap.map[y][x];
            }

            averageAltitude /= PATHMARGIN * 2;
            averageHumdity /= PATHMARGIN * 2;

            float absAvg = fabs((averageAltitude - (Biomes[CLEARING].minHeight + Biomes[GRASSLAND].minHeight) / 2) + (averageHumdity - (Biomes[CLEARING].minHumidity + Biomes[GRASSLAND].minHumidity) / 2));

            if(absAvg < bestAbsAvg) {
                bestAbsAvg = absAvg;

                if(i == 0) screen->verticalPath.start = x - LENGTH / 2;
                else screen->verticalPath.end = x - LENGTH / 2;
            }
        }
    }

    screen->horizontalPath.start = 0;
    screen->horizontalPath.end = 0;

    for(int i = 0; i < 2; i++) {
        float bestAbsAvg = 100;
        for(int y = WIDTH / 2 + 1; y < WIDTH + WIDTH / 2 - 1; y++) {
            float averageAltitude = 0;
            float averageHumdity = 0;

            for(int x = LENGTH / 2 - PATHMARGIN + (LENGTH * i); x < LENGTH / 2 + PATHMARGIN + (LENGTH * i); x++) {
                averageAltitude += expandedAltitudeMap.map[y][x];
                averageHumdity += expandedHumidityMap.map[y][x];
            }

            averageAltitude /= PATHMARGIN * 2;
            averageHumdity /= PATHMARGIN * 2;

            float absAvg = OptimalScore(averageAltitude, averageHumdity);

            if(absAvg < bestAbsAvg) {
                bestAbsAvg = absAvg;

                if(i == 0) screen->horizontalPath.start = y - WIDTH / 2;
                else screen->horizontalPath.end = y - WIDTH / 2;
            }
        }
    }


    float** biomeGrid = (float**)malloc(WIDTH * sizeof(float*));

    for (int y = 0; y < WIDTH; y++) {
        biomeGrid[y] = (float*)malloc(LENGTH * sizeof(float));

        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = OptimalScore(screen->biomeMap[y][x].minHeight, screen->biomeMap[y][x].minHumidity) * 10;
        }
    }

    asnode_t* horizontalPath = aStar(biomeGrid, WIDTH, LENGTH, 1, screen->horizontalPath.start, LENGTH - 2, screen->horizontalPath.end);

    for (int i = 0; i < WIDTH; ++i) {
        free(biomeGrid[i]);
    }
    
    free(biomeGrid);

    while(horizontalPath != NULL) {
        int x = horizontalPath->x;
        int y = horizontalPath->y;

        screen->biomeMap[y][x].biomeID = Path.biomeID;
        screen->biomeMap[y][x].type = Path.type;

        horizontalPath = horizontalPath->previous;
    }
}

float OptimalScore (float altitude, float humidity) {
    return fabs((altitude - (Biomes[CLEARING].minHeight + Biomes[GRASSLAND].minHeight) / 2) + (humidity - (Biomes[CLEARING].minHumidity + Biomes[GRASSLAND].minHumidity) / 2));
}

expandedmap_t MapExpander (wave_t wave[WAVENUM]) {
    expandedmap_t expandedMap;
    expandedMap.map[WIDTH * 2][LENGTH * 2];

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