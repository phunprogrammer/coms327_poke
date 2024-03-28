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
int changeScreen(screen_t* screen, char* cameFrom, int* updateScreen, vector_t* screenCoord, vector_t move);

#endif