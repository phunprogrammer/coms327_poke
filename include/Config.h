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

//Biomes
#define BIOMENUM 5
#define STRUCNUM 3
#define ENTITYNUM 7
#define TILENUM BIOMENUM + STRUCNUM + ENTITYNUM

// enum __attribute__ ((__packed__)) Tile {
//     FOREST,
//     MOUNTAIN,
//     CLEARING,
//     GRASSLAND,
//     OCEAN,
//     PATH,
//     POKEM,
//     POKEC,
//     PC,
//     HIKER,
//     RIVAL,
//     PACER,
//     WANDERER,
//     SENTRY,
//     EXPLORER
// };

typedef struct Vector {
    float x;
    float y;
} vector_t;

typedef struct Coord {
    int x;
    int y;
} coord_t;

typedef struct Gates {
    coord_t north;
    coord_t south;
    coord_t east;
    coord_t west;
} gates_t;

typedef struct Path {
    coord_t coord;
    int gCost;
} path_t;

typedef struct Paths {
    std::vector<path_t> horizontalPath;
    std::vector<path_t> verticalPath;
} paths_t;

typedef struct EntityMove {
    int entityIndex;
    vector_t coord;
    int priority;
} entityMove_t;

// typedef struct TileType {
//     enum Tile biomeID;
//     float minHeight;
//     float minHumidity;
//     char type;
//     int weight;
// } tileType_t;

// typedef struct EntityType {
//     tileType_t tile;
//     tileType_t originalTile;
//     vector_t coord;
//     int weightFactor[TILENUM];
//     path_t* entityPath;
//     path_t* (*getPath)(struct Screen* screen, struct EntityType* entity);
//     vector_t direction;
//     bool defeated;
// } entityType_t;

// typedef struct Screen {
//     tileType_t** biomeMap;
//     vector_t coord;
//     pathgates_t horizontalEndpoints;
//     pathgates_t verticalEndpoints;
//     path_t* horizontalPath;
//     path_t* verticalPath;
//     entityType_t pc;
//     entityType_t* npcs;
//     int npcSize;
//     pqueue_t* moveQueue;
//     int priority;
// } screen_t;

typedef struct Building {
    path_t* path;
    int vertical;
    int inverse;
} building_t;

extern volatile int initialized;

extern volatile int numNPC;

// extern const tileType_t Tiles[TILENUM];

// extern const entityType_t Entities[TILENUM];

void Initialize();

int Init_Entities();

#endif