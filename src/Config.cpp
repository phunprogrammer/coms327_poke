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
void Initialize() {
    if (initialized == 1)
        return;

    initialized = 1;

    srand(time(NULL));    
}