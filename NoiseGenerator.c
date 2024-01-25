#include "PerlinNoise.h"
#include "NoiseGenerator.h"

float* Generate (float scale, wave_t waves[WAVENUM], offset_t offset) {
    float noiseMap[WIDTH][HEIGHT] = { 0 };

    int i,j,k;

    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
            float posX = (float)i * scale + offset.x;
            float posY = (float)j * scale + offset.y;

            float normalization = 0.0f;

            for (k = 0; k < WAVENUM; k++) {
                noiseMap[i][j] += waves[k].amplitude * noise2(posX * waves[k].frequency + waves[k].seed, posY * waves[k].frequency + waves[k].seed);
                normalization += waves[k].amplitude;
            }

            noiseMap[i][j] /= normalization;
        }
    }

    return noiseMap;
}