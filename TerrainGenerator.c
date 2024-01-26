#include "NoiseGenerator.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h> 

void GenerateTerrain() {
    waves_t waves = GetWaves();
    offset_t offset = { 0, 0 };

    noisemap_t altitudeMap = Generate(1, waves.Altitude, offset);
    noisemap_t humidityMap = Generate(1, waves.Humidity, offset);
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