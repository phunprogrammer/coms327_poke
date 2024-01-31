#include "Config.h"

#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

screen_t ScreenGenerator(waves_t waves);
biomeType_t** GenerateTerrain(waves_t waves, screen_t* screen);
void FreeBiomeArray(biomeType_t** array, int width, int length);
biomeType_t ChooseBiome(float altitude, float humidity);
void UpdateOffset(int x, int y);
void GeneratePath(waves_t waves, screen_t* screen);
float OptimalScore (float altitude, float humidity);
expandedmap_t MapExpander (wave_t wave[WAVENUM]);

#endif