#ifndef CONFIG_H
#define CONFIG_H

#define WIDTH 21
#define LENGTH 80
#define WAVENUM 1

//Waves
#define AMPLITUDE (float[]){ 1, 1 }
#define FREQUENCY (float[]){ 0.05, 0.03 }

typedef struct Wave {
    float seed;
    float frequency;
    float amplitude;
} wave_t;

typedef struct Offset {
    float x;
    float y;
} offset_t;

void initialize();

void InitWaves(wave_t waves[WAVENUM]);

#endif