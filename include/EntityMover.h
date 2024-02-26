#include "Config.h"
#include "PQueue.h"

#ifndef ENTITYMOVER_H
#define ENTITYMOVER_H

int MovePC(screen_t* screen, vector_t move);
int GetAllNPCMoves(screen_t* screen, pqueue_t* moveq);
int PCController(screen_t* screen, char input);

#endif