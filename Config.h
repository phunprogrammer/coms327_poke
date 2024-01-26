#ifndef CONFIG_H
#define CONFIG_H

#define WIDTH 21
#define LENGTH 80
#define WAVENUM 2
#define BIOMENUM 5

//Waves
#define AMPLITUDE (float[]){ 1.1, 0.9, 1, 1 }
#define FREQUENCY (float[]){ 0.05, 0.07, 0.03, 0.03 }

//Biomes
enum Biome {
    FOREST,
    MOUNTAIN,
    CLEARING,
    GRASSLAND,
    OCEAN
};

typedef struct Wave {
    float seed;
    float frequency;
    float amplitude;
} wave_t;

typedef struct Waves {
    wave_t Altitude[WAVENUM];
    wave_t Humidity[WAVENUM];
} waves_t;

typedef struct Offset {
    float x;
    float y;
} offset_t;

typedef struct BiomeType {
    enum Biome biomeID;
    float minHeight;
    float minHumidity;
    char type;
} biomeType_t;

extern volatile int initialized;

extern const biomeType_t Biomes[BIOMENUM];

void Initialize();

waves_t GetWaves();

#endif