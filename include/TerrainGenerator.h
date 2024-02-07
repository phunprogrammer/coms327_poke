#include "Config.h"

#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

screen_t ScreenGenerator(waves_t waves);
int DestroyScreen(screen_t* screen);
tileType_t** GenerateTerrain(waves_t waves, screen_t* screen);
void FreeBiomeArray(tileType_t** array, int width);
tileType_t ChooseBiome(float altitude, float humidity);
void UpdateOffset(int x, int y);
void GeneratePath(waves_t waves, screen_t* screen);
void SwitchTile (tileType_t* tileA, tileType_t tileB);
int endPointSelector(pathgates_t* path, int width, int length, expandedmap_t altitudeMap, expandedmap_t humidityMap);
expandedmap_t MapExpander (wave_t wave[WAVENUM]);
int GenerateBuildings(screen_t* screen);
int isValidBuilding(screen_t* screen, int currX, int currY, int* value, int inverse, int vertical);
int ConstructBuilding(screen_t* screen, building_t* building, tileType_t tile);
int FirstFourDigits(float num);

#endif