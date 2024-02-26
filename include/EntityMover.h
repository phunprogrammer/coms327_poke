#include "Config.h"

#ifndef ENTITYMOVER_H
#define ENTITYMOVER_H

int MovePC(screen_t* screen, vector_t move);
//path_t* GetAllNPCMoves(screen_t* screen, entityType_t* entity);
int PCController(screen_t* screen, char input);

#endif