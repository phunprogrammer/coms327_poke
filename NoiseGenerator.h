#include "Config.h"

#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

typedef struct NoiseMap {
    float map[WIDTH][LENGTH];
} noisemap_t;

noisemap_t Generate (float scale, wave_t waves[WAVENUM], offset_t offset);

#endif