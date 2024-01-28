#include "NoiseGenerator.h"
#include "PPMGenerator.h"
#include "TerrainGenerator.h"
#include "Config.h"
#include <stdio.h>
#include <stdlib.h>

int main () {
    Initialize();
    waves_t waves = GetWaves();

    enum Biome PPMBiomeMap[WIDTH * SCREENS][LENGTH * SCREENS] = { 0 };

    for(int i = 0; i < SCREENS; i++) {
        for(int j = 0; j < SCREENS; j++) {
            UpdateOffset(j, i);
            noisemap_t tempMap = GenerateTerrain(waves);

            for(int y = WIDTH * i; y < WIDTH * (i + 1); y++) {
                for(int x = LENGTH * j; x < LENGTH * (j + 1); x++) {
                    PPMBiomeMap[y][x] = tempMap.map[y % WIDTH][x % LENGTH];
                }
            }
        }
    }

    PrintHeader(LENGTH * SCREENS, WIDTH * SCREENS, 255);

    for(int i = 0; i < WIDTH * SCREENS; i++) {
        for(int j = 0; j < LENGTH * SCREENS; j++) {
            PrintNext(PPMBiomeMap[i][j]);
        }
    }

    CloseFile();

    return 0;
}