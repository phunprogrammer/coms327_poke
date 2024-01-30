#include "NoiseGenerator.h"
#include "Config.h"
#include "TerrainGenerator.h"
#include "PPMGenerator.h"
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

static vector_t offset = { .x = 0, .y = 0 };

noisemap_t GenerateTerrain(waves_t waves) {
    noisemap_t biomeMap = { 0 };

    noisemap_t altitudeMap = Generate(1, waves.Altitude, offset);
    noisemap_t humidityMap = Generate(1, waves.Humidity, offset);

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            biomeType_t biome;

            if (x == 0 || y == 0 || y == WIDTH - 1 || x == LENGTH - 1)
                biome = Biomes[1];
            else
                biome = ChooseBiome(altitudeMap.map[y][x], humidityMap.map[y][x]);

            //printf("%c", biome.type);
            biomeMap.map[y][x] = (float)biome.biomeID;
        }
        //printf("\n");
    }
    PathGeneration(waves);

    return biomeMap;
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

void PathGeneration(waves_t waves) {
    noisemap_t altitudeMap = Generate(1, waves.Altitude, offset);
    expandedmap_t expandedAltitudeMap = MapExpander(waves.Altitude);
    expandedmap_t expandedHumidityMap = MapExpander(waves.Humidity);

    path_t bestVerticalPath = { .start = 0, .end = 0 };

    for(int i = 0; i < 2; i++) {
        float bestAbsAvg = 100;
        for(int x = LENGTH / 2 + 1; x < LENGTH + LENGTH / 2 - 1; x++) {
            float averageAltitude = 0;
            float averageHumdity = 0;
            for(int y = WIDTH / 2 - PATHMARGIN + (WIDTH * i); y < WIDTH / 2 + PATHMARGIN + (WIDTH * i); y++) {
                averageAltitude += expandedAltitudeMap.map[y][x];
                averageHumdity += expandedHumidityMap.map[y][x];
                expandedAltitudeMap.map[y][x] *= 3;
            }

            averageAltitude /= PATHMARGIN * 2;
            averageHumdity /= PATHMARGIN * 2;

            float absAvg = fabs((averageAltitude - (Biomes[CLEARING].minHeight + Biomes[GRASSLAND].minHeight) / 2) + (averageHumdity - (Biomes[CLEARING].minHumidity + Biomes[GRASSLAND].minHumidity) / 2));

            if(absAvg < bestAbsAvg) {
                bestAbsAvg = absAvg;

                if(i == 0) bestVerticalPath.start = x - LENGTH / 2;
                else bestVerticalPath.end = x - LENGTH / 2;
            }
        }
    }

    path_t bestHorizontalPath = { .start = 0, .end = 0 };

    for(int i = 0; i < 2; i++) {
        float bestAbsAvg = 100;
        for(int y = WIDTH / 2 + 1; y < WIDTH + WIDTH / 2 - 1; y++) {
            float averageAltitude = 0;
            float averageHumdity = 0;

            for(int x = LENGTH / 2 - PATHMARGIN + (LENGTH * i); x < LENGTH / 2 + PATHMARGIN + (WIDTH * i); x++) {
                averageAltitude += expandedAltitudeMap.map[y][x];
                averageHumdity += expandedHumidityMap.map[y][x];
                expandedAltitudeMap.map[y][x] *= 3;
            }

            averageAltitude /= PATHMARGIN * 2;
            averageHumdity /= PATHMARGIN * 2;

            float absAvg = fabs((averageAltitude - (Biomes[CLEARING].minHeight + Biomes[GRASSLAND].minHeight) / 2) + (averageHumdity - (Biomes[CLEARING].minHumidity + Biomes[GRASSLAND].minHumidity) / 2));

            if(absAvg < bestAbsAvg) {
                bestAbsAvg = absAvg;

                if(i == 0) bestHorizontalPath.start = y - WIDTH / 2;
                else bestHorizontalPath.end = y - WIDTH / 2;
            }
        }
    }
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