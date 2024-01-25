#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#define WIDTH 210
#define LENGTH 800
#define WAVENUM 2

typedef struct Wave {
    float seed;
    float frequency;
    float amplitude;
} wave_t;

typedef struct Offset {
    float x;
    float y;
} offset_t;

typedef struct NoiseMap {
    float map[WIDTH][LENGTH];
} noisemap_t;

noisemap_t Generate (float scale, wave_t waves[WAVENUM], offset_t offset);

#endif