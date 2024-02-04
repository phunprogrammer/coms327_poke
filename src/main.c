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

    if(GENERATEPPM)
        GeneratePPM(waves);

    UpdateOffset(MIDDLE, MIDDLE);
    screen_t screen = ScreenGenerator(waves);
    char input;

    do {
        for(int y = 0; y < WIDTH; y++) {
            for(int x = 0; x < LENGTH; x++) {
                printf("%c", screen.biomeMap[y][x].type);
            }
            printf("\n");
        } 
    } while ((input = getc(stdin)) != 'q');
    
    return 0;
}