#include "NoiseGenerator.h"
#include "Config.h"
#include "TerrainGenerator.h"
#include "PPMGenerator.h"
#include "AStar.h"
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <time.h>

/**
 * @brief Current offset of the world
 * 
 */
static vector_t offset = { .x = 0, .y = 0 };

/**
 * @brief Generates a screen with all components in it
 * 
 * @param waves Waves generated from getWaves();
 * @return screen_t 
 */
screen_t ScreenGenerator(waves_t waves) {
    screen_t screen = { .screenCoords = offset };

    GenerateTerrain(waves, &screen);
    GeneratePath(waves, &screen);
    RandomizeBuildings(&screen);
    BorderDetector(&screen);

    return screen;
}

/**
 * @brief Destroys screen and all malloced components
 * 
 * @param screen 
 * @return int 
 */
int DestroyScreen(screen_t* screen) {
    FreeBiomeArray(screen->biomeMap, WIDTH);

    path_t* horizontalPath = screen->horizontalPath;

    while (horizontalPath != NULL) {
        path_t* tempPath = horizontalPath;
        horizontalPath = horizontalPath->previous;
        free(tempPath);
    }

    path_t* verticalPath = screen->verticalPath;

    while (verticalPath != NULL) {
        path_t* tempPath = verticalPath;
        verticalPath = verticalPath->previous;
        free(tempPath);
    }

    return 0;
}

/**
 * @brief Detects if the square is a border and blocks off the gates
 * 
 * @param screen 
 * @return int 
 */
int BorderDetector(screen_t* screen) {
    if (offset.x == MAXSIZE * LENGTH)
        SwitchTile(&(screen->biomeMap[screen->horizontalEndpoints.end][LENGTH - 1]), Tiles[MOUNTAIN]);
    if (offset.y == MAXSIZE * WIDTH)
        SwitchTile(&(screen->biomeMap[WIDTH - 1][screen->verticalEndpoints.end]), Tiles[MOUNTAIN]);
    if (offset.x == MINSIZE * LENGTH)
        SwitchTile(&(screen->biomeMap[screen->horizontalEndpoints.start][0]), Tiles[MOUNTAIN]);
    if (offset.y == MINSIZE * WIDTH)
        SwitchTile(&(screen->biomeMap[0][screen->verticalEndpoints.start]), Tiles[MOUNTAIN]);

    return 0;
}

/**
 * @brief Generates terrain based off of the waves given
 * 
 * @param waves 
 * @param screen 
 * @return tileType_t** 
 */
tileType_t** GenerateTerrain(waves_t waves, screen_t* screen) {
    screen->biomeMap = (tileType_t**)malloc(WIDTH * sizeof(tileType_t*));

    for (int i = 0; i < WIDTH; ++i)
        screen->biomeMap[i] = (tileType_t*)malloc(LENGTH * sizeof(tileType_t));

    noisemap_t altitudeMap = GenerateNoise(1, waves.Altitude, offset);
    noisemap_t humidityMap = GenerateNoise(1, waves.Humidity, offset);

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            tileType_t biome = ChooseBiome(altitudeMap.map[y][x], humidityMap.map[y][x]);

            if ((x == 0 || y == 0 || y == WIDTH - 1 || x == LENGTH - 1) && biome.biomeID != FOREST && biome.biomeID != OCEAN) {
                SwitchTile(&biome, Tiles[MOUNTAIN]);
            }

            biome.minHeight = altitudeMap.map[y][x];
            biome.minHumidity = humidityMap.map[y][x];

            screen->biomeMap[y][x] = biome;
        }
    }

    return screen->biomeMap;
}

/**
 * @brief Frees the biome array in screen
 * 
 * @param array 
 * @param width 
 */
void FreeBiomeArray(tileType_t** array, int width) {
    for (int i = 0; i < width; ++i) {
        free(array[i]);
    }
    
    free(array);
}

/**
 * @brief Chooses the correct biome based off of altitude and humidity given
 * 
 * @param altitude 
 * @param humidity 
 * @return tileType_t 
 */
tileType_t ChooseBiome(float altitude, float humidity) {
    tileType_t compatibleBiomes[BIOMENUM];
    int numCompatible = 0;

    for (int i = 0; i < BIOMENUM; i++) {
        if (altitude >= Tiles[i].minHeight && humidity >= Tiles[i].minHumidity) {
            compatibleBiomes[numCompatible] = Tiles[i];
            numCompatible++;
        }
    }

    if (numCompatible == 0)
        return Tiles[OCEAN];
    
    int bestBiomeIndex = 0;
    float bestBiomeVal = 10.0;

    for (int i = 0; i < numCompatible; i++) {
        float difference = (altitude - compatibleBiomes[i].minHeight) + (humidity - compatibleBiomes[i].minHumidity);

        if (difference < bestBiomeVal) {
            bestBiomeVal = difference;
            bestBiomeIndex = i;
        }
    }

    return compatibleBiomes[bestBiomeIndex];
}

/**
 * @brief Updates the offset of the terrain generator
 * 
 * @param x 
 * @param y 
 */
void UpdateOffset(int x, int y) {
    offset.x = x * LENGTH;
    offset.y = y * WIDTH;
}

/**
 * @brief Generates the paths on the screen
 * 
 * @param waves 
 * @param screen 
 */
void GeneratePath(waves_t waves, screen_t* screen) {
    expandedmap_t expandedAltitudeMap = MapExpander(waves.Altitude);
    expandedmap_t expandedHumidityMap = MapExpander(waves.Humidity);

    endPointSelector(&(screen->verticalEndpoints), WIDTH, LENGTH, expandedAltitudeMap, expandedHumidityMap);

    for (int i = 0; i < PATHOFFSET; i++) {
        SwitchTile(&(screen->biomeMap[i][screen->verticalEndpoints.start]), Tiles[PATH]);
        SwitchTile(&(screen->biomeMap[(WIDTH - 1) - i][screen->verticalEndpoints.end]), Tiles[PATH]);
    }

    endPointSelector(&(screen->horizontalEndpoints), LENGTH, WIDTH, expandedAltitudeMap, expandedHumidityMap);

    for (int i = 0; i < PATHOFFSET; i++) {
        SwitchTile(&(screen->biomeMap[screen->horizontalEndpoints.start][i]), Tiles[PATH]);
        SwitchTile(&(screen->biomeMap[screen->horizontalEndpoints.end][(LENGTH - 1)- i]), Tiles[PATH]);
    }

    int** biomeGrid = (int**)malloc(WIDTH * sizeof(int*));

    for (int y = 0; y < WIDTH; y++) {
        biomeGrid[y] = (int*)malloc(LENGTH * sizeof(int));

        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = screen->biomeMap[y][x].weight;
        }
    }

    pqueue_t open;

    path_t* horizontalPath = aStar(biomeGrid, WIDTH, LENGTH, PATHOFFSET, screen->horizontalEndpoints.start, (LENGTH - 1) - PATHOFFSET, screen->horizontalEndpoints.end, &open);
    screen->horizontalPath = horizontalPath;

    while(horizontalPath != NULL) {
        int x = horizontalPath->coord.x;
        int y = horizontalPath->coord.y;
        
        biomeGrid[y][x] = Tiles[PATH].weight;
        SwitchTile(&(screen->biomeMap[y][x]), Tiles[PATH]);
        
        horizontalPath = horizontalPath->previous;
    }

    path_t* verticalPath = aStar(biomeGrid, WIDTH, LENGTH, screen->verticalEndpoints.start, PATHOFFSET, screen->verticalEndpoints.end, (WIDTH - 1) - PATHOFFSET, &open);
    screen->verticalPath = verticalPath;

    for (int i = 0; i < WIDTH; ++i) {
        free(biomeGrid[i]);
    }
    free(biomeGrid);

    while(verticalPath != NULL) {
        int x = verticalPath->coord.x;
        int y = verticalPath->coord.y;

        SwitchTile(&(screen->biomeMap[y][x]), Tiles[PATH]);

        verticalPath = verticalPath->previous;
    }
}

/**
 * @brief Switches tileA ptr on the screen with tileB
 * 
 * @param tileA 
 * @param tileB 
 */
void SwitchTile (tileType_t* tileA, tileType_t tileB) {
    float tempAltitude = tileA->minHeight;
    float tempHumidity = tileA->minHumidity;
    *tileA = tileB;
    tileA->minHeight = tempAltitude;
    tileA->minHumidity = tempHumidity;
}

/**
 * @brief Selects endpoints based on the average of between screens for paths
 * 
 * @param path pathgates of the screen
 * @param width 
 * @param length 
 * @param altitudeMap 
 * @param humidityMap 
 * @return int 
 */
int endPointSelector(pathgates_t* path, int width, int length, expandedmap_t altitudeMap, expandedmap_t humidityMap) {
    path->start = 0;
    path->end = 0;

    for(int i = 0; i < 2; i++) {
        float bestAbsAvg = length * Tiles[OCEAN].weight * 2;
        for(int x = length / 2 + PATHOFFSET; x < length + length / 2 - PATHOFFSET; x++) {
            int average = 0;
            for(int y = width / 2 - PATHMARGIN + (width * i); y < width / 2 + PATHMARGIN + (width * i); y++) {
                int j = y;
                int k = x;

                if(length == WIDTH) {
                    j = x;
                    k = y;
                }

                average += ChooseBiome(altitudeMap.map[j][k], humidityMap.map[j][k]).weight;
            }

            average /= PATHMARGIN * 2;

            float absAvg = average * length + (abs(length - x) / 4);

            if(absAvg < bestAbsAvg) {
                bestAbsAvg = absAvg;

                if(i == 0) path->start = x - length / 2;
                else path->end = x - length / 2;
            }
        }
    }

    return 0;
}

/**
 * @brief Expands the given map by 4x based off of the offset. Used for endpoint selector
 * 
 * @param wave 
 * @return expandedmap_t 
 */
expandedmap_t MapExpander (wave_t wave[WAVENUM]) {
    expandedmap_t expandedMap;

    vector_t pathOffsets[QUADRANT] = {
        [0] = { .x = LENGTH / -2 + offset.x, .y = WIDTH / -2 + offset.y },
        [1] =  { .x = (LENGTH - LENGTH / 2) + offset.x, .y = WIDTH / -2 + offset.y },
        [2] = { .x = LENGTH / -2 + offset.x, .y = (WIDTH - WIDTH / 2) + offset.y },
        [3] = { .x = (LENGTH - LENGTH / 2) + offset.x, .y = (WIDTH - WIDTH / 2) + offset.y }
    };

    vector_t translation[QUADRANT] = {
        [0] = { .x = 0, .y = 0 },
        [1] =  { .x = 1, .y = 0 },
        [2] = { .x = 0, .y = 1 },
        [3] = { .x = 1, .y = 1 }
    };

    for(int i = 0; i < QUADRANT; i++) {
        noisemap_t temp = GenerateNoise(1, wave, pathOffsets[i]);

        for(int y = 0; y < WIDTH; y++) {
            for(int x = 0; x < LENGTH; x++) {
                expandedMap.map[y + WIDTH * (int)translation[i].y][x + LENGTH * (int)translation[i].x] = temp.map[y][x];
            }
        }
    }

    return expandedMap;
}

/**
 * @brief Randomizes if the screen should generate a building.
 * 
 * @param screen 
 * @return int 
 */
int RandomizeBuildings(screen_t* screen) {
    int chance = 100;
    int dist = abs((int)(offset.x / LENGTH) - MAXSIZE / 2) + abs((int)(offset.y / WIDTH) - MAXSIZE / 2);

    if (dist > 0)
        chance = pow(0.975, dist - 155) + 5;

    srand(FirstFourDigits(screen->biomeMap[0][0].minHeight));

    if((rand() % 100) < chance)
        GenerateBuildings(screen);

    return 0;
}

/**
 * @brief Generates buildings for the given screen
 * 
 * @param screen 
 * @return int 
 */
int GenerateBuildings(screen_t* screen) {
    path_t* verticalPath = screen->verticalPath;
    path_t* horizontalPath = screen->horizontalPath;

    srand(screen->biomeMap[0][0].minHeight);

    int horizontalBias = rand() % LENGTH;
    int verticalBias = rand() % WIDTH;

    pqueue_t buildingQueue;
    pq_init(&buildingQueue);

    while(horizontalPath != NULL) {
        int x = horizontalPath->coord.x;
        int y = horizontalPath->coord.y;

        for(int i = -1; i <= 1; i += 2) {
            int value = 0;

            if (isValidBuilding(screen, x, y, &value, i, 0)) {
                building_t* building = (building_t *)malloc(sizeof(building_t));
                building->path = horizontalPath;
                building->inverse = i;
                building->vertical = 0;
                pq_enqueue(&buildingQueue, building, value + abs(horizontalBias - x) + rand() % 10);
            }
        }

        horizontalPath = horizontalPath->previous;
    }

    while(verticalPath != NULL) {
        int x = verticalPath->coord.x;
        int y = verticalPath->coord.y;

        for(int i = -1; i < 0; i += 2) {
            int value = 0;

            if (isValidBuilding(screen, x, y, &value, i, 1)) {
                building_t* building = (building_t *)malloc(sizeof(building_t));
                building->path = verticalPath;
                building->inverse = i;
                building->vertical = 1;
                pq_enqueue(&buildingQueue, building, value + abs(verticalBias - y) + rand() % 10);
            }
        }

        verticalPath = verticalPath->previous;
    }

    void* node;
    pq_dequeue(&buildingQueue, &node);
    building_t* pokemart = (building_t*)node;
    ConstructBuilding(screen, pokemart, Tiles[POKEM]); 
    free(pokemart);

    int constructed = 1;

    while(constructed == 1 || pq_size(&buildingQueue) == 0){
        pq_dequeue(&buildingQueue, &node);
        building_t* pokecenter = (building_t*)node;
        constructed = ConstructBuilding(screen, pokecenter, Tiles[POKEC]);
        free(pokecenter);
    }

    pq_destroy(&buildingQueue);

    return 0;
}

/**
 * @brief Detects if the given position is a valid position for a building
 * 
 * @param screen 
 * @param currX x position of the path
 * @param currY y position of the path
 * @param value random bias onn the path to make it not favor the first available position
 * @param inverse determines if it is on the left or right side of the path
 * @param vertical determins if it is a vertical path of a horizontal path
 * @return int 
 */
int isValidBuilding(screen_t* screen, int currX, int currY, int* value, int inverse, int vertical) {
    vector_t buildingOffset[QUADRANT] = {
        [0] = { .x = 0, .y = 1 },
        [1] =  { .x = 0, .y = 2 },
        [2] = { .x = 1, .y = 2 },
        [3] = { .x = 1, .y = 1 }
    };

    for (int j = 0; j < QUADRANT; j++) {
        int newX = 0;
        int newY = 0;

        if (!vertical) {
            newX = currX + buildingOffset[j].x;
            newY = currY + buildingOffset[j].y * inverse;
        }
        else {
            newX = currX + buildingOffset[j].y * inverse;
            newY = currY + buildingOffset[j].x;
        }

        if((newX <= 0 && newY && 0 && newX >= LENGTH - 1 && newY >= WIDTH - 1) || screen->biomeMap[newY][newX].biomeID == PATH)
            return 0;

        *value += screen->biomeMap[newY][newX].weight;
    }

    if(!vertical && screen->biomeMap[currY][currX + 1].biomeID != PATH)
        return 0;

    if(vertical && screen->biomeMap[currY + 1][currX].biomeID != PATH)
        return 0;

    *value /= QUADRANT;

    return 1;
}

/**
 * @brief Constructs the building with the building struct
 * 
 * @param screen 
 * @param building 
 * @param tile 
 * @return int 
 */
int ConstructBuilding(screen_t* screen, building_t* building, tileType_t tile) {
    vector_t buildingOffset[QUADRANT] = {
        [0] = { .x = 0, .y = 1 },
        [1] =  { .x = 0, .y = 2 },
        [2] = { .x = 1, .y = 2 },
        [3] = { .x = 1, .y = 1 }
    };

    vector_t placed[QUADRANT];

    int currX = building->path->coord.x;
    int currY = building->path->coord.y;
    int inverse = building->inverse;
    int vertical = building->vertical;

    for (int j = 0; j < QUADRANT; j++) {
        int newX = 0;
        int newY = 0;

        if (!vertical) {
            newX = currX + buildingOffset[j].x;
            newY = currY + buildingOffset[j].y * inverse;
        }
        else {
            newX = currX + buildingOffset[j].y * inverse;
            newY = currY + buildingOffset[j].x;
        }

        if (screen->biomeMap[newY][newX].biomeID == POKEM)
            return 1;

        
        placed[j].x = newX;
        placed[j].y = newY;
    }

    for (int j = 0; j < QUADRANT; j++) {
        SwitchTile(&(screen->biomeMap[(int)(placed[j].y)][(int)(placed[j].x)]), tile);
    }

    return 0;
}

/**
 * @brief Returns the first four digits of a float value as an 4 digit int
 * 
 * @param num 
 * @return int 
 */
int FirstFourDigits(float num) {
    float abs = fabs(num);

    int firstFourDigits = (int)(abs * pow(10, 3 - floor(log10(abs))));

    return firstFourDigits;
}