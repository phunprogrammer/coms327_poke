
#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#define HEIGHT 21
#define WIDTH 80
#define WAVENUM 5

typedef struct Wave {
    float seed;
    float frequency;
    float amplitude;
} wave_t;

typedef struct Offset {
    float x;
    float y;
} offset_t;

float* Generate (float scale, wave_t waves[WAVENUM], offset_t offset);

#endif