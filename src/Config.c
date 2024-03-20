#include "Config.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

volatile int initialized = 0;

volatile int numNPC = DEFAULTENTITIES;

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
    [POKEC] = { POKEC, .minHeight = 0, .minHumidity = 0, .type = 'C', .weight = 100 },
    [PC] = { PC, .minHeight = 0, .minHumidity = 0, .type = '@', .weight = 100 },
    [HIKER] = { HIKER, .minHeight = 0, .minHumidity = 0, .type = 'h', .weight = 100 },
    [RIVAL] = { RIVAL, .minHeight = 0, .minHumidity = 0, .type = 'r', .weight = 100 },
    [PACER] = { PACER, .minHeight = 0, .minHumidity = 0, .type = 'p', .weight = 100 },
    [WANDERER] = { WANDERER, .minHeight = 0, .minHumidity = 0, .type = 'w', .weight = 100 },
    [SENTRY] = { SENTRY, .minHeight = 0, .minHumidity = 0, .type = 's', .weight = 100 },
    [EXPLORER] = { EXPLORER, .minHeight = 0, .minHumidity = 0, .type = 'e', .weight = 100 }
};

#define GENERICWEIGHTS {  0, 0, 10, 20, 0, 10, 0, 0, 100, 100, 100, 100, 100, 100, 100 }

const entityType_t Entities[TILENUM] = {
    [PC] =    { .tile = Tiles[PC],    .weightFactor = {  0,  0, 10, 20, 0, 10, 10, 10, 100, 100, 100, 100, 100, 100, 100 } },
    [HIKER] = { .tile = Tiles[HIKER], .weightFactor = { 15, 15, 10, 15, 0, 10,  0,  0, 100, 100, 100, 100, 100, 100, 100 } },
    [RIVAL] = { .tile = Tiles[RIVAL], .weightFactor = {  0,  0, 10, 20, 0, 10,  0,  0, 100, 100, 100, 100, 100, 100, 100 } },
    [PACER] = { .tile = Tiles[PACER], .weightFactor = GENERICWEIGHTS },
    [WANDERER] = { .tile = Tiles[WANDERER], .weightFactor = GENERICWEIGHTS },
    [SENTRY] = { .tile = Tiles[SENTRY], .weightFactor = GENERICWEIGHTS },
    [EXPLORER] = { .tile = Tiles[EXPLORER], .weightFactor = GENERICWEIGHTS }
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
waves_t GetWaves(int* seed) {
    waves_t waves;

    int i = 0;
    *seed = rand();
    //*seed = 549798832;

    for (; i < WAVENUM; i++) {
        waves.Altitude[i].amplitude = AMPLITUDE[i];
        waves.Altitude[i].frequency = FREQUENCY[i];
        waves.Altitude[i].seed = *seed / 10000.0;
    }

    for (int j = 0; j < WAVENUM; j++) {
        waves.Humidity[j].amplitude = AMPLITUDE[i];
        waves.Humidity[j].frequency = FREQUENCY[i];
        waves.Humidity[j].seed = *seed / 10000.0;
    }

    return waves;
}
