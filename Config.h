#ifndef CONFIG_H
#define CONFIG_H

#define WIDTH 21
#define LENGTH 80
#define SCREENS 10 //Max 17
#define WAVENUM 2
#define BIOMENUM 5

//Path
#define PATHSCALE 1

//Waves
#define AMPLITUDE (float[]){  1.3, 0.5,  1.0,  0.5 }
#define FREQUENCY (float[]){ 0.05, 0.08, 0.03, 0.06 }

//Biomes
enum Biome {
    FOREST,
    MOUNTAIN,
    CLEARING,
    GRASSLAND,
    OCEAN
};

typedef struct NoiseMap {
    float map[WIDTH][LENGTH];
} noisemap_t;

typedef struct Wave {
    float seed;
    float frequency;
    float amplitude;
} wave_t;

typedef struct Waves {
    wave_t Altitude[WAVENUM];
    wave_t Humidity[WAVENUM];
} waves_t;

typedef struct Vector {
    float x;
    float y;
} vector_t;

typedef struct Path {
    int start;
    int end;
} path_t;

typedef struct BiomeType {
    enum Biome biomeID;
    float minHeight;
    float minHumidity;
    char type;
} biomeType_t;

typedef struct Screen {
    biomeType_t biomeTile[WIDTH][LENGTH];
    vector_t screenCoords;
    path_t horizontalPath;
    path_t verticalPath;
} screen_t;

extern volatile int initialized;

extern const biomeType_t Biomes[BIOMENUM];

void Initialize();

waves_t GetWaves();

#endif