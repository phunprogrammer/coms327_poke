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
    { FOREST, -0.5, 0.35, '^', 50 },
    { MOUNTAIN, 0.25, -0.6, '%', 90 },
    { CLEARING, -0.5, -0.35, '.', 15 },
    { GRASSLAND, -0.5, 0.05, ':', 25 },
    { OCEAN, -0.8, -0.8, '~', 100 },
    { PATH, 0, 0, '#', -5 },
    { POKEM, 0, 0, 'M', 100 },
    { POKEC, 0, 0, 'C', 100 },
    { PC, 0, 0, '@', 100 },
    { HIKER, 0, 0, 'h', 100 },
    { RIVAL, 0, 0, 'r', 100 },
    { PACER, 0, 0, 'p', 100 },
    { WANDERER, 0, 0, 'w', 100 },
    { SENTRY, 0, 0, 's', 100 },
    { EXPLORER, 0, 0, 'e', 100 }
};

#define GENERICWEIGHTS {  0, 0, 10, 20, 0, 10, 0, 0, 100, 100, 100, 100, 100, 100, 100 }

const entityType_t Entities[TILENUM] = {
    { Tiles[SENTRY], Tiles[SENTRY], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[SENTRY], Tiles[SENTRY], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[SENTRY], Tiles[SENTRY], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[SENTRY], Tiles[SENTRY], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[SENTRY], Tiles[SENTRY], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[SENTRY], Tiles[SENTRY], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[SENTRY], Tiles[SENTRY], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[SENTRY], Tiles[SENTRY], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[PC], Tiles[PC], {0, 0}, {0, 0, 10, 20, 0, 10, 10, 10, 100, 100, 100, 100, 100, 100, 100}, nullptr, nullptr, {0, 0}, false },
    { Tiles[HIKER], Tiles[HIKER], {0, 0}, {15, 15, 10, 15, 0, 10, 0, 0, 100, 100, 100, 100, 100, 100, 100}, nullptr, nullptr, {0, 0}, false },
    { Tiles[RIVAL], Tiles[RIVAL], {0, 0}, {0, 0, 10, 20, 0, 10, 0, 0, 100, 100, 100, 100, 100, 100, 100}, nullptr, nullptr, {0, 0}, false },
    { Tiles[PACER], Tiles[PACER], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[WANDERER], Tiles[WANDERER], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[SENTRY], Tiles[SENTRY], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false },
    { Tiles[EXPLORER], Tiles[EXPLORER], {0, 0}, GENERICWEIGHTS, nullptr, nullptr, {0, 0}, false }
};

const float FREQUENCY[] = { 0.1f, 0.05f, 0.06f, 0.08f };
const float AMPLITUDE[] = { 1.0f, 1.5f, 1.0f, 0.8f };

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
