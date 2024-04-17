/**
 * @brief Configuration file for the whole game
 * @author phun
 * @date 2/8/24
 */
#include <vector>

#ifndef CONFIG_H
#define CONFIG_H

//PPM
#define SCREENS 11
#define GENERATEPPM 0

//Terminal
#define TERM_WIDTH 24
#define TERM_LENGTH 80

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

enum Command : int {
    CONTINUE,
    NOTHING,
    END,
    DEFEATED
};

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

typedef struct Building {
    path_t path;
    int vertical;
    int inverse;
} building_t;

typedef struct Wave {
    float seed;
    float frequency;
    float amplitude;
} wave_t;

typedef struct Waves {
    std::vector<wave_t> Height;
    std::vector<wave_t> Humidity;
} waves_t;

extern volatile int initialized;

extern volatile int numNPC;

extern int seed;

extern waves_t waves;

void Initialize();

#endif