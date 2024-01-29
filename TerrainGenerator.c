#include "NoiseGenerator.h"
#include "Config.h"
#include "TerrainGenerator.h"
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

static vector_t offset = { .x = 0, .y = 0 };

noisemap_t GenerateTerrain(waves_t waves) {
    noisemap_t biomeMap = { 0 };

    noisemap_t altitudeMap = Generate(PATHSCALE, waves.Altitude, offset);
    noisemap_t humidityMap = Generate(PATHSCALE, waves.Humidity, offset);

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

void PathGeneration(waves_t waves, noisemap_t* biomeMap) {
    // vector_t tempOffset = { .x = (PATHSCALE * LENGTH - LENGTH) / -2 + offset.x, .y = (PATHSCALE * WIDTH - WIDTH) / -2 + offset.y };

    // noisemap_t altitudeMap = Generate(PATHSCALE, waves.Altitude, tempOffset);
    // noisemap_t humidityMap = Generate(PATHSCALE, waves.Humidity, tempOffset);

    // int yMargin = ceil(WIDTH - WIDTH / PATHSCALE);
    // int xMargin = ceil(LENGTH - LENGTH / PATHSCALE);

    // path_t bestVerticalPath = { 0, 0 }; 
    // float optimalNorth = 100;
    // float optimalSouth = 100;
    // path_t bestHorizontalPath = { 0, 0 }; 
    // float optimalWest = 100;
    // float optimalEast = 100;
    
    // for(int x = xMargin + 1; x < (WIDTH - xMargin) + 1; x++) {
    //     float averageAltitude = 0;
    //     float averageHumidity = 0;

    //     for(int y = 0; y < yMargin; y++) {
    //         averageAltitude += altitudeMap.map[y][x];
    //         averageHumidity += humidityMap.map[y][x];
    //     }

    //     averageAltitude /= yMargin;
    //     averageHumidity /= yMargin;

    //     float value = abs(averageAltitude - Biomes[CLEARING].minHeight) + (averageHumidity - Biomes[CLEARING].minHumidity);

    //     if (value < optimalNorth) {
    //         optimalNorth = value;
    //         bestVerticalPath = { .start = x };
    //     }
    // }
}