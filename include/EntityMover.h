#include "Config.h"
#include "PQueue.h"

#ifndef ENTITYMOVER_H
#define ENTITYMOVER_H

int ValidMove(screen_t* screen, entityType_t* entity, vector_t move);
int InBounds(entityType_t* entity);
int MoveEntity(screen_t* screen, entityType_t* entity, vector_t move);
int AssignPathFunc(entityType_t* entity);
int GetAllNPCMoves(screen_t* screen, pqueue_t* moveq, int currentPriority);
int AddPathToQ(pqueue_t* moveq, screen_t* screen, int entityindex, int currentPriority);

path_t* GetHikerPath (screen_t* screen, entityType_t* entity);
path_t* GetRivalPath (screen_t* screen, entityType_t* entity);
path_t* GetPacerPath (screen_t* screen, entityType_t* entity);
path_t* GetWandererPath (screen_t* screen, entityType_t* entity);
path_t* GetExplorerPath (screen_t* screen, entityType_t* entity);

#endif