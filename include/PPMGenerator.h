#include "Config.h"
#include <stdio.h>

#ifndef PPMGENERATOR_H
#define PPMGENERATOR_H

void GeneratePPM (int length, int width, int height, waves_t waves);

void PrintNext(enum Biome biome);

void PrintHeader(int length, int width, int height);

void CloseFile();

#endif