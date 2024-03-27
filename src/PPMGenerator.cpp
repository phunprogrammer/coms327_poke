#include "NoiseGenerator.h"
#include "Config.h"
#include "TerrainGenerator.h"
#include "PPMGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static FILE *file;

/**
 * @brief Generates a PPM map for given waves. Settings can be changed in config
 * 
 * @param waves 
 */
void GeneratePPM(waves_t waves) {
    time_t startTime, endTime;
    startTime = clock();

    screen_t screen;
    PrintHeader(LENGTH * SCREENS, WIDTH * SCREENS, 255);
    
    int startX = MIDDLEX - SCREENS / 2;
    int endX = MIDDLEX + SCREENS / 2 + SCREENS % 2;
    int startY = MIDDLEY - SCREENS / 2;
    int endY = MIDDLEY + SCREENS / 2 + SCREENS % 2;

    for(int i = startY; i < endY; i++) {
        enum Tile* PPMMap = (enum Tile *)(malloc(sizeof(enum Tile) * WIDTH * LENGTH * SCREENS));
        
        for(int j = startX; j < endX; j++) {
            UpdateOffset(&screen, j, i);
            ScreenGenerator(&screen, waves);

            for(int y = 0; y < WIDTH; y++) {
                for(int x = LENGTH * (j - (MIDDLEX - SCREENS / 2)); x < LENGTH * ((j - (MIDDLEX - SCREENS / 2)) + 1); x++) {
                    PPMMap[y * LENGTH * SCREENS + x] = screen.biomeMap[y][x % LENGTH].biomeID;
                }
            }
            
            DestroyScreen(&screen);
        }

        for(int k = 0; k < WIDTH; k++) {
            for(int l = 0; l < LENGTH * SCREENS; l++) {
                PrintNext(PPMMap[k * LENGTH * SCREENS + l]);
            }
        }

        free(PPMMap);
        
        printf("%.2f percent done\n", ((float)((i + 1) - startY) / SCREENS) * 100.0);
    }

    CloseFile();
    system("pnmtopng map.ppm > map.png");

    endTime = clock();

    printf("Time to Gen: %.2f\n", (double)(endTime - startTime) / (double)CLOCKS_PER_SEC);
}

/**
 * @brief Prints different color based on tile
 * 
 * @param biome 
 */
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
            fprintf(file, "%c%c%c", 43, 135, 240);
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

/**
 * @brief Print based on a float number
 * 
 * @param value 
 */
void PrintNum(float value) {
    int newValue = (int)((value + 0.7) * 182);
    fprintf(file, "%d %d %d\n", newValue, newValue, newValue);
}

/**
 * @brief Prints the head of the PPM file. CALL BEFORE GENERATING PPM
 * 
 * @param length 
 * @param width 
 * @param height 
 */
void PrintHeader(int length, int width, int height) {
    file = fopen("map.ppm", "wb");

    fprintf(file, "P6 %d %d %d\n", length, width, height);
}

/**
 * @brief Closes PPM file. CALL AFTER GENERATING PPM
 * 
 */
void CloseFile() {
    fclose(file);
}