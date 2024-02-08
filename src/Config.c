#include "Config.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

volatile int initialized = 0;

/**
 * @brief Tile Assignment
 * 
 */
const tileType_t Tiles[TILENUM] = {
    [FOREST] = { FOREST, .minHeight = -0.5, .minHumidity = 0.35, .type = '^', .weight = 50 },
    [MOUNTAIN] = { MOUNTAIN, .minHeight = 0.25, .minHumidity = -0.6, .type = '%', .weight = 90 },
    [CLEARING] = { CLEARING, .minHeight = -0.5, .minHumidity = -0.35, .type = '.', .weight = 15 },
    [GRASSLAND] = { GRASSLAND, .minHeight = -0.5, .minHumidity = 0.05, .type = ':', .weight = 25 },
    [OCEAN] = { OCEAN, .minHeight = -0.8, .minHumidity = -0.8, .type = '~', .weight = 100 },
    [PATH] = { PATH, .minHeight = 0, .minHumidity = 0, .type = '#', .weight = -5 },
    [POKEM] = { POKEM, .minHeight = 0, .minHumidity = 0, .type = 'M', .weight = 100 },
    [POKEC] = { POKEC, .minHeight = 0, .minHumidity = 0, .type = 'C', .weight = 100 }
};

/**
 * @brief Inialization. CALL BEFORE ANYTHING
 * 
 */
void Initialize() {
    if (initialized == 1)
        return;

    initialized = 1;

    srand(time(NULL));     
}

/**
 * @brief Get the Waves (altitude waves and humidity) for the noise map
 * 
 * @return waves_t 
 */
waves_t GetWaves() {
    waves_t waves;

    int i = 0;
    int seed = rand();
    
    printf("%d\n", seed);

    for (; i < WAVENUM; i++) {
        waves.Altitude[i].amplitude = AMPLITUDE[i];
        waves.Altitude[i].frequency = FREQUENCY[i];
        waves.Altitude[i].seed = seed / 10000.0;
    }

    for (int j = 0; j < WAVENUM; j++) {
        waves.Humidity[j].amplitude = AMPLITUDE[i];
        waves.Humidity[j].frequency = FREQUENCY[i];
        waves.Humidity[j].seed = seed / 10000.0;
    }

    return waves;
}
