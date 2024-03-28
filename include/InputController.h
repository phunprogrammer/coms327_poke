#include "Config.h"

#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

vector_t MovePC(screen_t* screen, char input);
int ListTrainers(screen_t* screen);
int EnterBuilding(screen_t* screen);
int EnterBattle(screen_t* screen, int entityIndex);
void InitColors();
int PrintMap(screen_t* screen, int* line);
int fly(vector_t* coord);

#endif