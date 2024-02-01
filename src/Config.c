#include "Config.h"

#include <stdio.h>

#include <time.h>
#include <stdlib.h>

volatile int initialized = 0;

//Biome Assignment
const biomeType_t Biomes[BIOMENUM] = {
    [0] = { FOREST, .minHeight = -0.5, .minHumidity = 0.35, .type = '^' },
    [1] = { MOUNTAIN, .minHeight = 0.25, .minHumidity = -0.6, .type = '%' },
    [2] = { CLEARING, .minHeight = -0.5, .minHumidity = -0.35, .type = '.' },
    [3] = { GRASSLAND, .minHeight = -0.5, .minHumidity = 0.05, .type = ':' },
    [4] = { OCEAN, .minHeight = -0.65, .minHumidity = -0.65, .type = '~' }
};

const biomeType_t Path = { PATH, .minHeight = -0.5, .minHumidity = -0.3, .type = '@' };

void Initialize() {
    if (initialized == 1)
        return;

    initialized = 1;

    srand(time(NULL));     
}

waves_t GetWaves() {
    waves_t waves;

    int i = 0;
    int seed = rand() /*/ 10000*/;

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