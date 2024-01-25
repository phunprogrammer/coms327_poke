#include "NoiseGenerator.h"
#include <stdio.h>

void GeneratePPM (int length, int width, int height) {
    FILE *fptr;
    fptr = fopen("noisemap.ppm", "w");
    
    fprintf(fptr, "P3 %d %d %d\n", length, width, height);

    wave_t altitudeWave;
    altitudeWave.amplitude = 1;
    altitudeWave.frequency = 0.05;
    altitudeWave.seed = 3200;

    wave_t humidityWave;
    humidityWave.amplitude = 1;
    humidityWave.frequency = 0.03;
    humidityWave.seed = 60;

    wave_t waves[WAVENUM] = { altitudeWave };

    offset_t offset;
    offset.x = 0;
    offset.y = 0;

    noisemap_t noiseMap = Generate(1, waves, offset);

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            int value = (int)((noiseMap.map[y][x] + 0.6) * 212.0);

            fprintf(fptr, "%d %d %d\n", value, value, value);
        }
    }

    fclose(fptr);
}

int main () {
    GeneratePPM(LENGTH, WIDTH, 255);
    return 0;
}