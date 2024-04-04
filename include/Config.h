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

typedef struct Coord {
    int x;
    int y;

    bool operator==(const Coord& compare) const {
        return this->x == compare.x && this->y == compare.y;
    }

    bool operator!=(const Coord& compare) const {
        return this->x != compare.x || this->y != compare.y;
    }

    bool operator<(const Coord& compare) const {
        if (this->x == compare.x) {
            return this->y < compare.y;
        }
        return this->x < compare.x;
    }
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
    coord_t coord;
    int priority;
} entityMove_t;

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

typedef struct Building {
    path_t path;
    int vertical;
    int inverse;
} building_t;

extern volatile int initialized;

extern volatile int numNPC;

void Initialize();

#endif