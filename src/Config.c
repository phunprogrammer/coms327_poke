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
    [POKEC] = { POKEC, .minHeight = 0, .minHumidity = 0, .type = 'C', .weight = 100 },
    [PC] = { PC, .minHeight = 0, .minHumidity = 0, .type = '@', .weight = 100 },
    [HIKER] = { HIKER, .minHeight = 0, .minHumidity = 0, .type = 'H', .weight = 100 },
    [RIVAL] = { RIVAL, .minHeight = 0, .minHumidity = 0, .type = 'R', .weight = 100 }
};

#define HIKERWEIGHT (int){ 15, 15, 10, 15, 0, 10, 50, 50 }
const entityType_t Entities[TILENUM] = {
    [PC] = { .tile = Tiles[PC], .weightFactor = {  0, 0, 10, 20, 0, 10, 10, 10 } },
    [HIKER] = { .tile = Tiles[HIKER], .weightFactor = { 15, 15, 10, 15, 0, 10, 50, 50 } },
    [RIVAL] = { .tile = Tiles[RIVAL], .weightFactor = {  0, 0, 10, 20, 0, 10, 50, 50 } }
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

    //Init_Entities(); 
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
    seed = 549798832;
    
    printf("seed: %d\n", seed);

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

// int Init_Entities() {
//     for(int i = 0; i < ENTITYNUM; i++) {
//         Entities[i].tile = Tiles[BIOMENUM + STRUCNUM + i];

//         for(int j = 0; j < BIOMENUM + STRUCNUM; j++) {
//             Entities[i].weightFactor[j] = PCWEIGHT[j];
//         }
//     }

//     return 0;
// }
