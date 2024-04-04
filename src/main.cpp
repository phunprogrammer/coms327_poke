#include "Config.h"
#include "Screen.h"
#include "Entities.h"
#include <iostream>

int main() {
    Initialize();

    int seed;
    waves_t waves = Screen::GetWaves(&seed);
    coord_t coord = { 200, 200 };

    Screen screen = Screen(waves, coord);

    std::vector<std::vector<TerrainTile>> terrainMap = screen.getTerrainMap();
    std::vector<std::vector<StructureTile>> structureMap = screen.getStructureMap();
    std::vector<std::vector<EntityTile*>> entityMap = screen.getEntityMap();

    for(int i = 0; i < (int)terrainMap.size(); i++) {
        for(int j = 0; j < (int)terrainMap[i].size(); j++) {
            if(entityMap[i][j] != nullptr) {
                std::cout << (char)(entityMap[i][j]->getEntity());
                continue;
            }
            if(structureMap[i][j].getStructure() != NULL_STRUCT) {
                std::cout << (char)(structureMap[i][j].getStructure());
                continue;
            }
            std::cout << (char)(terrainMap[i][j].getTerrain());
        }
        std::cout << '\n';
    }

    return 0;
}