#include "NoiseGenerator.h"
#include "Config.h"
#include <stdio.h>

void GeneratePPM (int length, int width, int height) {
    FILE *fptr;
    fptr = fopen("noisemap.ppm", "w");
    
    fprintf(fptr, "P3 %d %d %d\n", length, width, height);

    offset_t offset;
    offset.x = 0;
    offset.y = 0;

    wave_t waves[WAVENUM];
    InitWaves(waves);

    noisemap_t noiseMap = Generate(1, waves, offset);

    float max = -100;
    float min = 100;

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            int value = (int)((noiseMap.map[y][x] + 0.6) * 212.0);
            
            if (noiseMap.map[y][x] > max)
                max = noiseMap.map[y][x];
            
            if (noiseMap.map[y][x] < min)
                min = noiseMap.map[y][x];

            fprintf(fptr, "%d %d %d\n", value, value, value);
        }
    }

    printf("Max: %f, min: %f\n", max, min);

    fclose(fptr);
}

int main () {
    initialize();
    GeneratePPM(LENGTH, WIDTH, 255);
    return 0;
}