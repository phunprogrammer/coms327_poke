#include "Config.h"

#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

void GenerateTerrain(waves_t waves);
biomeType_t ChooseBiome(float altitude, float humidity);

#endif