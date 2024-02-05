#include "NoiseGenerator.h"
#include "Config.h"
#include "TerrainGenerator.h"
#include "PPMGenerator.h"
#include "AStar.h"
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

static vector_t offset = { .x = 0, .y = 0 };

screen_t ScreenGenerator(waves_t waves) {
    screen_t screen = { .screenCoords = offset };

    GenerateTerrain(waves, &screen);
    GeneratePath(waves, &screen);

    int chance = 100;
    int dist = offset.x / LENGTH + offset.y / WIDTH;

    if (dist < 400)
        //chance = 0.11 * dist + 5;
        chance = pow(0.99, dist * -1) + 5;
    else if (dist > 400)
        //chance = -0.11 * (float)(dist - 800) + 5;
        chance = pow(0.99, dist - 800) + 5;

    srand(FirstFourDigits(screen.biomeMap[0][0].minHeight));

    if((rand() % 100) < chance)
        GenerateBuildings(&screen);

    if (offset.x == MAXSIZE * LENGTH)
        SwitchTile(&(screen.biomeMap[screen.horizontalEndpoints.end][LENGTH - 1]), Tiles[MOUNTAIN]);
    if (offset.y == MAXSIZE * WIDTH)
        SwitchTile(&(screen.biomeMap[0][screen.verticalEndpoints.start]), Tiles[MOUNTAIN]);
    if (offset.x == 0)
        SwitchTile(&(screen.biomeMap[screen.horizontalEndpoints.start][0]), Tiles[MOUNTAIN]);
    if (offset.y == 0)
        SwitchTile(&(screen.biomeMap[WIDTH - 1][screen.verticalEndpoints.end]), Tiles[MOUNTAIN]);
    
    return screen;
}

tileType_t** GenerateTerrain(waves_t waves, screen_t* screen) {
    screen->biomeMap = (tileType_t**)malloc(WIDTH * sizeof(tileType_t*));

    for (int i = 0; i < WIDTH; ++i)
        screen->biomeMap[i] = (tileType_t*)malloc(LENGTH * sizeof(tileType_t));

    noisemap_t altitudeMap = Generate(1, waves.Altitude, offset);
    noisemap_t humidityMap = Generate(1, waves.Humidity, offset);

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

void FreeBiomeArray(tileType_t** array, int width) {
    for (int i = 0; i < width; ++i) {
        free(array[i]);
    }
    
    free(array);
}

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

void UpdateOffset(int x, int y) {
    offset.x = x * LENGTH;
    offset.y = y * WIDTH;
}

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


    float** biomeGrid = (float**)malloc(WIDTH * sizeof(float*));

    for (int y = 0; y < WIDTH; y++) {
        biomeGrid[y] = (float*)malloc(LENGTH * sizeof(float));

        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = screen->biomeMap[y][x].weight;
        }
    }

    asnode_t* horizontalPath = aStar(biomeGrid, WIDTH, LENGTH, PATHOFFSET, screen->horizontalEndpoints.start, (LENGTH - 1) - PATHOFFSET, screen->horizontalEndpoints.end, PATHOFFSET - 1, screen->horizontalEndpoints.start);
    screen->horizontalPath = horizontalPath;

    while(horizontalPath != NULL) {
        int x = horizontalPath->x;
        int y = horizontalPath->y;

        SwitchTile(&(screen->biomeMap[y][x]), Tiles[PATH]);

        horizontalPath = horizontalPath->previous;
    }

    
    for (int y = 0; y < WIDTH; y++) {
        biomeGrid[y] = (float*)malloc(LENGTH * sizeof(float));

        for (int x = 0; x < LENGTH; x++) {
            biomeGrid[y][x] = screen->biomeMap[y][x].weight;
        }
    }

    asnode_t* verticalPath = aStar(biomeGrid, WIDTH, LENGTH, screen->verticalEndpoints.start, PATHOFFSET, screen->verticalEndpoints.end, (WIDTH - 1) - PATHOFFSET, screen->verticalEndpoints.start, PATHOFFSET - 1);
    screen->verticalPath = verticalPath;

    for (int i = 0; i < WIDTH; ++i) {
        free(biomeGrid[i]);
    }
    free(biomeGrid);

    while(verticalPath != NULL) {
        int x = verticalPath->x;
        int y = verticalPath->y;

        SwitchTile(&(screen->biomeMap[y][x]), Tiles[PATH]);

        verticalPath = verticalPath->previous;
    }
}

void SwitchTile (tileType_t* tileA, tileType_t tileB) {
    float tempAltitude = tileA->minHeight;
    float tempHumidity = tileA->minHumidity;
    *tileA = tileB;
    tileA->minHeight = tempAltitude;
    tileA->minHumidity = tempHumidity;
}

int endPointSelector(path_t* path, int width, int length, expandedmap_t altitudeMap, expandedmap_t humidityMap) {
    path->start = 0;
    path->end = 0;

    for(int i = 0; i < 2; i++) {
        float bestAbsAvg = length * 100;
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
        noisemap_t temp = Generate(1, wave, pathOffsets[i]);

        for(int y = 0; y < WIDTH; y++) {
            for(int x = 0; x < LENGTH; x++) {
                expandedMap.map[y + WIDTH * (int)translation[i].y][x + LENGTH * (int)translation[i].x] = temp.map[y][x];
            }
        }
    }

    return expandedMap;
}

int GenerateBuildings(screen_t* screen) {
    asnode_t* verticalPath = (asnode_t *)(screen->verticalPath);
    asnode_t* horizontalPath = (asnode_t *)(screen->horizontalPath);

    srand(screen->biomeMap[0][0].minHeight);

    int horizontalBias = rand() % LENGTH;
    int verticalBias = rand() % WIDTH;

    pqueue_t buildingQueue;
    pq_init(&buildingQueue);

    while(horizontalPath != NULL) {
        int x = horizontalPath->x;
        int y = horizontalPath->y;

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
        int x = verticalPath->x;
        int y = verticalPath->y;

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

    int constructed = 1;
    while(constructed == 1){
        pq_dequeue(&buildingQueue, &node);
        building_t* pokecenter = (building_t*)node;
        constructed = ConstructBuilding(screen, pokecenter, Tiles[POKEC]);
    }

    pq_destroy(&buildingQueue);

    return 0;
}

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

        if((newX <= 0 && newY <= 0 && newX >= LENGTH - 1 && newY >= WIDTH - 1) || screen->biomeMap[newY][newX].biomeID == PATH)
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

int ConstructBuilding(screen_t* screen, building_t* building, tileType_t tile) {
    vector_t buildingOffset[QUADRANT] = {
        [0] = { .x = 0, .y = 1 },
        [1] =  { .x = 0, .y = 2 },
        [2] = { .x = 1, .y = 2 },
        [3] = { .x = 1, .y = 1 }
    };

    vector_t placed[QUADRANT];

    int currX = (*(asnode_t*)(building->path)).x;
    int currY = (*(asnode_t*)(building->path)).y;
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

int FirstFourDigits(float num) {
    float abs = fabs(num);

    int firstFourDigits = (int)(abs * pow(10, 3 - floor(log10(abs))));

    return firstFourDigits;
}