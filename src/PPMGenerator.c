#include "NoiseGenerator.h"
#include "Config.h"
#include "TerrainGenerator.h"
#include "PPMGenerator.h"
#include <stdio.h>

static FILE *file;

void GeneratePPM(int length, int width, int height, waves_t waves) {
    if (initialized == 0)
        printf("Initialize the game first!");

    PrintHeader(length, width, height);

    vector_t offset = { 0, 0 };

    noisemap_t altitudeMap = Generate(1, waves.Altitude, offset);
    noisemap_t humidityMap = Generate(1, waves.Humidity, offset);

    float minAlt = 100;
    float maxAlt = -100;

    float minHum = 100;
    float maxHum = -100;

    for (int y = 0; y < WIDTH; y++) {
        for (int x = 0; x < LENGTH; x++) {
            if (altitudeMap.map[y][x] < minAlt) {
                minAlt = altitudeMap.map[y][x];
            }

            if (altitudeMap.map[y][x] > maxAlt) {
                maxAlt = altitudeMap.map[y][x];
            }

            if (humidityMap.map[y][x] < minHum) {
                minHum = humidityMap.map[y][x];
            }

            if (humidityMap.map[y][x] > maxHum) {
                maxHum = humidityMap.map[y][x];
            }
            
            PrintNext(ChooseBiome(altitudeMap.map[y][x], humidityMap.map[y][x]).biomeID);
        }
    }

    fclose(file);

    printf("Max Altitude: %f, Min Altitude: %f\nMax Humidity: %f, Min Humidity: %f\n", maxAlt, minAlt, maxHum, minHum);
}

void PrintNext(enum Tile biome) {
    switch (biome) {
        case 0:
            fprintf(file, "%d %d %d\n", 24, 115, 0);
            break;
        case 1:
            fprintf(file, "%d %d %d\n", 80, 80, 80);
            break;
        case 2:
            fprintf(file, "%d %d %d\n", 70, 216, 32);
            break;
        case 3:
            fprintf(file, "%d %d %d\n", 50, 170, 29);
            break;
        case 4:
            fprintf(file, "%d %d %d\n", 0, 110, 236);
            break;
        case 5:
            fprintf(file, "%d %d %d\n", 255, 0, 0);
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
    file = fopen("noisemap.ppm", "w");

    fprintf(file, "P3 %d %d %d\n", length, width, height);
}

void CloseFile() {
    fclose(file);
}