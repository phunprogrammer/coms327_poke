/**
 * @brief Generates a noise map using perlin noise
 * @author phun
 * @date 2/8/24
 */

#include "Config.h"

#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

noisemap_t GenerateNoise (float scale, wave_t waves[WAVENUM], vector_t offset);

#endif