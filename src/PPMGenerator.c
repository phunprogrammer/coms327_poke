#include "NoiseGenerator.h"
#include "Config.h"
#include "TerrainGenerator.h"
#include "PPMGenerator.h"
#include <stdio.h>

static FILE *file;

void PrintNext(enum Tile biome) {
    switch (biome) {
        case FOREST:
            fprintf(file, "%d %d %d\n", 24, 115, 0);
            break;
        case MOUNTAIN:
            fprintf(file, "%d %d %d\n", 80, 80, 80);
            break;
        case CLEARING:
            fprintf(file, "%d %d %d\n", 70, 216, 32);
            break;
        case GRASSLAND:
            fprintf(file, "%d %d %d\n", 50, 170, 29);
            break;
        case OCEAN:
            fprintf(file, "%d %d %d\n", 0, 110, 236);
            break;
        case PATH:
            fprintf(file, "%d %d %d\n", 209, 148, 56);
            break;
        case POKEM:
            fprintf(file, "%d %d %d\n", 255, 0, 0);
            break;
        case POKEC:
            fprintf(file, "%d %d %d\n", 0, 0, 255);
            break;
        default:
            fprintf(file, "%d %d %d\n", 0, 0, 0);
            break;
    }
}

void PrintNum(float value) {
    int newValue = (int)((value + 0.7) * 182);
    fprintf(file, "%d %d %d\n", newValue, newValue, newValue);
}

void PrintHeader(int length, int width, int height) {
    file = fopen("map.ppm", "w");

    fprintf(file, "P3 %d %d %d\n", length, width, height);
}

void CloseFile() {
    fclose(file);
}