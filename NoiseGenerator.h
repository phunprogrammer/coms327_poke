#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#define WIDTH 21
#define LENGTH 80
#define WAVENUM 1

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
    float map[LENGTH][WIDTH];
} noisemap_t;

noisemap_t Generate (float scale, wave_t waves[WAVENUM], offset_t offset);

#endif