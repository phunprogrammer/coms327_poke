#include "Config.h"
#include <stdio.h>

static int initialized = 0;

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

void InitWaves(wave_t waves[WAVENUM]) {
    if (initialized == 0)
        printf("Initialize the game first!");

    for (int i = 0; i < WAVENUM; i++) {
        waves[i].amplitude = AMPLITUDE[i];
        waves[i].frequency = FREQUENCY[i];
        waves[i].seed = 0;
    }
}