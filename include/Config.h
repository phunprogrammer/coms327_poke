/**
 * @brief Configuration file for the whole game
 * @author phun
 * @date 2/8/24
 */
#include <vector>
#include "PQueue.h"

#ifndef CONFIG_H
#define CONFIG_H

//PPM
#define SCREENS 11
#define GENERATEPPM 0

//Screen
#define WIDTH 21
#define LENGTH 80
#define MIDDLEX 200
#define MIDDLEY 200
#define MAXSIZE 400
#define MINSIZE 0
#define DEFAULTENTITIES 10
#define DEVMODE 0

//Menu
#define MENUMARGIN 4

//Path
#define PATHMARGIN 3
#define QUADRANT 4
#define PATHOFFSET 2

//Waves
#define WAVENUM 2
extern const float FREQUENCY[];
extern const float AMPLITUDE[];

//Biomes
#define BIOMENUM 5
#define STRUCNUM 3
#define ENTITYNUM 7
#define TILENUM BIOMENUM + STRUCNUM + ENTITYNUM
enum __attribute__ ((__packed__)) Tile {
    FOREST,
    MOUNTAIN,
    CLEARING,
    GRASSLAND,
    OCEAN,
    PATH,
    POKEM,
    POKEC,
    PC,
    HIKER,
    RIVAL,
    PACER,
    WANDERER,
    SENTRY,
    EXPLORER
};

struct Screen;

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

typedef struct PathGates {
    int start;
    int end;
} pathgates_t;

typedef struct Path {
    vector_t coord;
    int gCost;
    struct Path* previous;
} path_t;

typedef struct EntityMove {
    int entityIndex;
    vector_t coord;
    int priority;
} entityMove_t;

typedef struct TileType {
    enum Tile biomeID;
    float minHeight;
    float minHumidity;
    char type;
    int weight;
} tileType_t;

typedef struct EntityType {
    tileType_t tile;
    tileType_t originalTile;
    vector_t coord;
    int weightFactor[TILENUM];
    path_t* entityPath;
    path_t* (*getPath)(struct Screen* screen, struct EntityType* entity);
    vector_t direction;
    bool defeated;
} entityType_t;

typedef struct Screen {
    tileType_t** biomeMap;
    vector_t screenCoords;
    pathgates_t horizontalEndpoints;
    pathgates_t verticalEndpoints;
    path_t* horizontalPath;
    path_t* verticalPath;
    entityType_t pc;
    entityType_t* npcs;
    int npcSize;
    pqueue_t* moveQueue;
} screen_t;

typedef struct Building {
    path_t* path;
    int vertical;
    int inverse;
} building_t;

extern volatile int initialized;

extern volatile int numNPC;

extern const tileType_t Tiles[TILENUM];

extern const entityType_t Entities[TILENUM];

void Initialize();

waves_t GetWaves(int* seed);

int Init_Entities();

#endif