#include "NoiseGenerator.h"
#include <stdio.h>

void GeneratePPM (int width, int length, int height) {
    remove("noisemap.ppm");
    FILE *fptr;
    fptr = fopen("noisemap.ppm", "w");
    
    fprintf(fptr, "%d %d %d\n", width, length, height);

    wave_t altitudeWave;
    altitudeWave.amplitude = 1;
    altitudeWave.frequency = 0.05;
    altitudeWave.seed = 56;

    wave_t humidityWave;
    humidityWave.amplitude = 1;
    humidityWave.frequency = 0.03;
    humidityWave.seed = 60;

    wave_t waves[WAVENUM] = { altitudeWave };

    offset_t offset;
    offset.x = 0;
    offset.y = 0;

    noisemap_t noiseMap = Generate(1, waves, offset);

    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            fprintf(fptr, "%f %f %f\n", noiseMap.map[i][j], noiseMap.map[i][j], noiseMap.map[i][j]);
        }
    }

    fclose(fptr);
}

int main () {
    GeneratePPM(WIDTH, LENGTH, 255);
    return 0;
}