#include "Config.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

volatile int initialized = 0;

volatile int numNPC = DEFAULTENTITIES;

/**
 * @brief Inialization. CALL BEFORE ANYTHING
 * 
 */
void Initialize(int& seed) {
    if (initialized == 1)
        return;

    initialized = 1;
    // seed = time(NULL);
    seed = 1712700259;

    srand(seed);    

    return;
}