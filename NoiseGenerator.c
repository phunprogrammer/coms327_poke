#include "PerlinNoise.h"
#include "NoiseGenerator.h"
#include <stdio.h>

noisemap_t Generate (float scale, wave_t waves[WAVENUM], offset_t offset) {
    noisemap_t noiseMap;
    int x,y,k;

    for (y = 0; y < WIDTH; y++) {
        for (x = 0; x < LENGTH; x++) {
            noiseMap.map[y][x] = 0;
        }
    }

    for (y = 0; y < WIDTH; y++) {
        for (x = 0; x < LENGTH; x++) {
            float posX = (float)x * scale + offset.x;
            float posY = (float)y * scale + offset.y;

            float normalization = 0.0f;

            for (k = 0; k < WAVENUM; k++) {
                noiseMap.map[y][x] += waves[k].amplitude * noise2(posX * waves[k].frequency + waves[k].seed, posY * waves[k].frequency + waves[k].seed);
                normalization += waves[k].amplitude;
            }

            noiseMap.map[y][x] /= normalization;
        }
    }

    return noiseMap;
}