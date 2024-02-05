#include "NoiseGenerator.h"
#include "PPMGenerator.h"
#include "TerrainGenerator.h"
#include "Config.h"
#include "PQueue.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main () {
    Initialize();
    
    waves_t waves = GetWaves();

    if(GENERATEPPM)
        GeneratePPM(waves);

    int currX = MIDDLE;
    int currY = MIDDLE;

    UpdateOffset(currX, currY);
    screen_t screen;
    char input = 0;

    do {
        switch (input) {
            case 'n':
                UpdateOffset(currX, (currY = fmax(--currY, 0)));
                break;
            case 'e':
                UpdateOffset((currX = fmin(++currX, MAXSIZE)), currY);
                break;
            case 's':
                UpdateOffset(currX, (currY = fmin(++currY, MAXSIZE)));
                break;
            case 'w':
                UpdateOffset((currX = fmax(--currX, 0)), currY);
                break;
            case 'f':
                int x, y;
                scanf("%d %d", &x, &y);
                UpdateOffset((currX = (int)fmax(fmin(MIDDLE + x, MAXSIZE), MINSIZE)), (currY = (int)fmax(fmin(MIDDLE + y, MAXSIZE), MINSIZE)));
                break;
            case 0:
                break;
            case '\n':
                continue;
            default:
                printf("ERROR: Not a valid command\n");
                continue;
        }
        
        screen = ScreenGenerator(waves);

        for(int y = 0; y < WIDTH; y++) {
            for(int x = 0; x < LENGTH; x++) {
                printf("%c", screen.biomeMap[y][x].type);
            }
            printf("\n");
        } 
        printf("(%d, %d)\n", currX - MIDDLE, currY - MIDDLE);

    } while ((input = getc(stdin)) != 'q');
    
    return 0;
}