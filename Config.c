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

    for (int i = 0; i < WAVENUM; i++) {
        waves.Altitude[i].amplitude = AMPLITUDE[0];
        waves.Altitude[i].frequency = FREQUENCY[0];
        waves.Altitude[i].seed = 0;

        waves.Humidity[i].amplitude = AMPLITUDE[1];
        waves.Humidity[i].frequency = FREQUENCY[1];
        waves.Humidity[i].seed = 0;
    }

    return waves;
}