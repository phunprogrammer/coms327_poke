#include "Config.h"
#include <stdio.h>

#ifndef PPMGENERATOR_H
#define PPMGENERATOR_H

void GeneratePPM (int length, int width, int height);

void ColorTile(FILE *file, enum Biome biome);

#endif