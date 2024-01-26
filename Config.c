#include "Config.h"
#include <stdio.h>

volatile int initialized = 0;

//Biome Assignment
const biomeType_t Biomes[BIOMENUM] = {
    [0] = { FOREST, .minHeight = 0.2, .minHumidity = 0.4 },
    [1] = { MOUNTAIN, .minHeight = 0.5, .minHumidity = 0 },
    [2] = { CLEARING, .minHeight = 0.2, .minHumidity = 0.2 },
    [3] = { GRASSLAND, .minHeight = 0.2, .minHumidity = 0.3 },
    [4] = { OCEAN, .minHeight = 0, .minHumidity = 0 }
};

void Initialize() {
    if (initialized == 1)
        return;

    initialized = 1;
}

waves_t GetWaves() {
    if (initialized == 0)
        printf("Initialize the game first!");

    waves_t waves;
    int i = 0;

    for (; i < WAVENUM; i++) {
        waves.Altitude[i].amplitude = AMPLITUDE[i];
        waves.Altitude[i].frequency = FREQUENCY[i];
        waves.Altitude[i].seed = 0;
    }

    for (int j = 0; j < WAVENUM; j++) {
        waves.Humidity[j].amplitude = AMPLITUDE[i];
        waves.Humidity[j].frequency = FREQUENCY[i];
        waves.Humidity[j].seed = 0;
    }

    return waves;
}