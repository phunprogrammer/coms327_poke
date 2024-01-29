#include "Config.h"

#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

noisemap_t GenerateTerrain(waves_t waves);
biomeType_t ChooseBiome(float altitude, float humidity);
void UpdateOffset(int x, int y);
void PathGeneration(waves_t waves, noisemap_t* biomeMap);

#endif