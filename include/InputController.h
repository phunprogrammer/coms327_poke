#include "Config.h"

#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

int MovePC(screen_t* screen, char input);
int ListTrainers(screen_t* screen);
int EnterBuilding(screen_t* screen);
int EnterBattle(screen_t* screen, int entityIndex);
void InitColors();
int DefeatedWeight(entityType_t* entity);
int PrintMap(screen_t* screen, int* line);

#endif