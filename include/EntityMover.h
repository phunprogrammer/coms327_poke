#include "Config.h"
#include "PQueue.h"

#ifndef ENTITYMOVER_H
#define ENTITYMOVER_H

int MoveEntity(screen_t* screen, entityType_t* entity, vector_t move);
int GetAllNPCMoves(screen_t* screen, pqueue_t* moveq);
int PCController(screen_t* screen, char input);

#endif