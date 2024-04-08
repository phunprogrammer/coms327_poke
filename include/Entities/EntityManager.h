#include "Config.h"

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

class Screen;
class EntityTile;
enum Entity : char;

class EntityManager {
    private:
        Screen& screen;
    public:
        EntityManager(Screen& screen);
        int SpawnAllNPC();
        EntityTile* SpawnNPC(Entity entity);
};

#endif