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
        int SpawnPC(coord_t coord);
        int SpawnNPC(Entity entity);
        int SpawnAllNPC();
};

#endif