#include "Config.h"
#include <stdio.h>

#ifndef PPMGENERATOR_H
#define PPMGENERATOR_H

void GeneratePPM(waves_t waves);

void PrintNext(enum Tile biome);

void PrintHeader(int length, int width, int height);

void CloseFile();

#endif