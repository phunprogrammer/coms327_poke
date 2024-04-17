#include "Config.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

volatile int initialized = 0;

volatile int numNPC = DEFAULTENTITIES;

int seed;

waves_t waves;

/**
 * @brief Inialization. CALL BEFORE ANYTHING
 * 
 */
void Initialize() {
    if (initialized == 1)
        return;

    initialized = 1;
    seed = time(NULL);
    seed = 1713316398;

    srand(seed);    

    return;
}