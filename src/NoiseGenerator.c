#include "PerlinNoise.h"
#include "NoiseGenerator.h"
#include "Config.h"
#include <stdio.h>

/**
 * @brief Generates a noise map for the size
 * 
 * @param scale Zoom in or out in
 * @param waves Different waves that determine the properties of the noise map
 * @param offset Offset the noise map by a certain x and y
 * @return noisemap_t 
 */
noisemap_t Generate (float scale, wave_t waves[WAVENUM], vector_t offset) {
    if (initialized == 0)
        printf("Initialize the game first!");
        
    noisemap_t noiseMap;
    int x,y,k;

    //Reset 2D noise map
    for (y = 0; y < WIDTH; y++) {
        for (x = 0; x < LENGTH; x++) {
            noiseMap.map[y][x] = 0;
        }
    }

    //Assign a value to each float in the noise map
    for (y = 0; y < WIDTH; y++) {
        for (x = 0; x < LENGTH; x++) {
            //Get the x and y based on the scale and offset
            float posX = (float)x * scale + offset.x;
            float posY = (float)y * scale + offset.y;

            float normalization = 0.0f;

            //For each wave
            for (k = 0; k < WAVENUM; k++) {
                noiseMap.map[y][x] += waves[k].amplitude * noise2(posX * waves[k].frequency + waves[k].seed, posY * waves[k].frequency + waves[k].seed);
                normalization += waves[k].amplitude;
            }

            noiseMap.map[y][x] /= normalization;
        }
    }

    return noiseMap;
}