#include "Config.h"
#include <stdio.h>

static int initialized = 0;

void initialize() {
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
    }
}