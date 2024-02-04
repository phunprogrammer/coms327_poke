#ifndef CONFIG_H
#define CONFIG_H

#define WIDTH 21
#define LENGTH 80
#define SCREENS 5 //Max 17
#define WAVENUM 2
#define BIOMENUM 5
#define TILENUM 6

//Path
#define PATHMARGIN WIDTH / 5
#define QUADRANT 4
#define PATHOFFSET 2

//Waves
#define AMPLITUDE (float[]){  1.0, 1.5,  1.0,  0.8 }
#define FREQUENCY (float[]){ 0.1, 0.05, 0.06, 0.08 }

//Biomes
enum Tile {
    FOREST,
    MOUNTAIN,
    CLEARING,
    GRASSLAND,
    OCEAN,
    PATH,
    POKEC,
    POKEM
};

typedef struct NoiseMap {
    float map[WIDTH][LENGTH];
} noisemap_t;

typedef struct ExpandedMap {
    float map[WIDTH * 2][LENGTH * 2];
} expandedmap_t;

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

typedef struct tileType {
    enum Tile biomeID;
    float minHeight;
    float minHumidity;
    int weight;
    char type;
} tileType_t;

typedef struct Screen {
    tileType_t** biomeMap;
    vector_t screenCoords;
    path_t horizontalPath;
    path_t verticalPath;
} screen_t;

extern volatile int initialized;

extern const tileType_t Tiles[TILENUM];

void Initialize();

waves_t GetWaves();

#endif