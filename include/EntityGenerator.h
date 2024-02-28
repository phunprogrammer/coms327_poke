#include "Config.h"

#ifndef ENTITYGENERATOR_H
#define ENTITYGENERATOR_H

int InitSize(screen_t* screen, int argc, char *argv[]);
int RandomizePC(screen_t* screen);
int SpawnNPC(screen_t* screen, enum Tile entity);
int SetEntity(screen_t* screen, entityType_t* entity, int x, int y, enum Tile entityID);
int GenWeightMap(screen_t* screen, entityType_t entity);
int SpawnAllNPC(screen_t* screen);

#endif