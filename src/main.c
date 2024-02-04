#include "NoiseGenerator.h"
#include "PPMGenerator.h"
#include "TerrainGenerator.h"
#include "Config.h"
#include "PQueue.h"
#include <stdio.h>
#include <stdlib.h>

int main () {
    Initialize();
    waves_t waves = GetWaves();

    screen_t screen = ScreenGenerator(waves);
    for(int y = 0; y < WIDTH; y++) {
        for(int x = 0; x < LENGTH; x++) {
            printf("%c", screen.biomeMap[y][x].type);
        }
        printf("\n");
    }

    PrintHeader(LENGTH * SCREENS, WIDTH * SCREENS, 255);

    for(int i = 0; i < SCREENS; i++) {
        enum Tile PPMBiomeMap[WIDTH][LENGTH * SCREENS] = { 0 };
        
        for(int j = 0; j < SCREENS; j++) {
            UpdateOffset(j, i);
            screen_t screen = ScreenGenerator(waves);

            for(int y = 0; y < WIDTH; y++) {
                for(int x = LENGTH * j; x < LENGTH * (j + 1); x++) {
                    PPMBiomeMap[y][x] = screen.biomeMap[y][x % LENGTH].biomeID;
                }
            }
            
            FreeBiomeArray(screen.biomeMap, WIDTH);
        }

        for(int k = 0; k < WIDTH; k++) {
            for(int l = 0; l < LENGTH * SCREENS; l++) {
                PrintNext(PPMBiomeMap[k][l]);
            }
        }
    }

    CloseFile();
    
    return 0;
}