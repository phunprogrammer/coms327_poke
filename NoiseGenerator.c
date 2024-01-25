#include "PerlinNoise.h"
#include "NoiseGenerator.h"
#include <stdio.h>

noisemap_t Generate (float scale, wave_t waves[WAVENUM], offset_t offset) {
    noisemap_t noiseMap;
    int i,j,k;

    for (i = 0; i < LENGTH; i++) {
        for (j = 0; j < WIDTH; j++) {
            noiseMap.map[i][j] = 0;
        }
    }

    for (i = 0; i < LENGTH; i++) {
        for (j = 0; j < WIDTH; j++) {
            float posX = (float)i * scale + offset.x;
            float posY = (float)j * scale + offset.y;

            float normalization = 0.0f;

            for (k = 0; k < WAVENUM; k++) {
                noiseMap.map[i][j] += waves[k].amplitude * noise2(posX * waves[k].frequency + waves[k].seed, posY * waves[k].frequency + waves[k].seed);
                normalization += waves[k].amplitude;
            }

            noiseMap.map[i][j] /= normalization;
        }
    }

    return noiseMap;
}