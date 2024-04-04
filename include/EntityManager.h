#include "Config.h"

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

class Screen;
enum Entity : char;

class EntityManager {
    private:
        Screen& screen;
    public:
        EntityManager(Screen& screen);
        int RandomizePC();
        int PlacePC(char cameFrom);
        int SpawnNPC(Entity entity);
        int SpawnAllNPC();
        int SpawnEntity(Entity entity, coord_t coord);
};

#endif