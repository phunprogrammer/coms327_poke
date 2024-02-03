#include "Config.h"

#include <stdio.h>

#include <time.h>
#include <stdlib.h>

volatile int initialized = 0;

//Biome Assignment
const tileType_t Tiles[TILENUM] = {
    [FOREST] = { FOREST, .minHeight = -0.5, .minHumidity = 0.35, .type = '^', .weight = 30 },
    [MOUNTAIN] = { MOUNTAIN, .minHeight = 0.25, .minHumidity = -0.6, .type = '%', .weight = 60 },
    [CLEARING] = { CLEARING, .minHeight = -0.5, .minHumidity = -0.4, .type = '.', .weight = 10 },
    [GRASSLAND] = { GRASSLAND, .minHeight = -0.5, .minHumidity = 0.05, .type = ':', .weight = 15 },
    [OCEAN] = { OCEAN, .minHeight = -1, .minHumidity = -1, .type = '~', .weight = 60 },
    [PATH] = { PATH, .minHeight = -0.5, .minHumidity = -0.2, .type = '#', .weight = 0 }
};

void Initialize() {
    if (initialized == 1)
        return;

    initialized = 1;

    srand(time(NULL));     
}

waves_t GetWaves() {
    waves_t waves;

    int i = 0;
    int seed = rand() / 10000;

    for (; i < WAVENUM; i++) {
        waves.Altitude[i].amplitude = AMPLITUDE[i];
        waves.Altitude[i].frequency = FREQUENCY[i];
        waves.Altitude[i].seed = seed;
    }

    for (int j = 0; j < WAVENUM; j++) {
        waves.Humidity[j].amplitude = AMPLITUDE[i];
        waves.Humidity[j].frequency = FREQUENCY[i];
        waves.Humidity[j].seed = seed;
    }

    return waves;
}
