#include "NoiseGenerator.h"
#include "Config.h"
#include "TerrainGenerator.h"
#include "PPMGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static FILE *file;

void GeneratePPM(waves_t waves) {
    time_t startTime, endTime;
    startTime = clock();

    screen_t screen;
    PrintHeader(LENGTH * SCREENS, WIDTH * SCREENS, 255);
    int start = MIDDLE - SCREENS / 2;
    int end = MIDDLE + SCREENS / 2 + SCREENS % 2;

    for(int i = start; i < end; i++) {
        enum Tile* PPMMap = (enum Tile *)(malloc(sizeof(enum Tile) * WIDTH * LENGTH * SCREENS));
        
        for(int j = start; j < end; j++) {
            UpdateOffset(j, i);
            screen = ScreenGenerator(waves);

            for(int y = 0; y < WIDTH; y++) {
                for(int x = LENGTH * (j - (MIDDLE - SCREENS / 2)); x < LENGTH * ((j - (MIDDLE - SCREENS / 2)) + 1); x++) {
                    PPMMap[y * LENGTH * SCREENS + x] = screen.biomeMap[y][x % LENGTH].biomeID;
                }
            }
            
            FreeBiomeArray(screen.biomeMap, WIDTH);
        }

        for(int k = 0; k < WIDTH; k++) {
            for(int l = 0; l < LENGTH * SCREENS; l++) {
                PrintNext(PPMMap[k * LENGTH * SCREENS + l]);
            }
        }

        free(PPMMap);
    }

    CloseFile();

    endTime = clock();

    printf("Time to Gen: %.2f\n", (double)(endTime - startTime) / (double)CLOCKS_PER_SEC);
}

void PrintNext(enum Tile biome) {
    switch (biome) {
        case FOREST:
            fprintf(file, "%c%c%c", 24, 115, 0);
            break;
        case MOUNTAIN:
            fprintf(file, "%c%c%c", 80, 80, 80);
            break;
        case CLEARING:
            fprintf(file, "%c%c%c", 70, 216, 32);
            break;
        case GRASSLAND:
            fprintf(file, "%c%c%c", 50, 170, 29);
            break;
        case OCEAN:
            fprintf(file, "%c%c%c", 0, 110, 236);
            break;
        case PATH:
            fprintf(file, "%c%c%c", 209, 148, 56);
            break;
        case POKEM:
            fprintf(file, "%c%c%c", 255, 0, 0);
            break;
        case POKEC:
            fprintf(file, "%c%c%c", 0, 0, 255);
            break;
        default:
            fprintf(file, "%c%c%c", 0, 0, 0);
            break;
    }
}

void PrintNum(float value) {
    int newValue = (int)((value + 0.7) * 182);
    fprintf(file, "%d %d %d\n", newValue, newValue, newValue);
}

void PrintHeader(int length, int width, int height) {
    file = fopen("map.ppm", "wb");

    fprintf(file, "P6 %d %d %d\n", length, width, height);
}

void CloseFile() {
    fclose(file);
}