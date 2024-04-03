#include "Config.h"
#include "Screen.h"
#include <iostream>

int main() {
    Initialize();

    int seed;
    waves_t waves = Screen::GetWaves(&seed);
    coord_t coord = { 0, 0 };

    Screen screen = Screen(waves, coord);

    std::vector<std::vector<TerrainTile>> terrainMap = screen.getTerrainMap();

    for(int i = 0; i < (int)terrainMap.size(); i++) {
        for(int j = 0; j < (int)terrainMap[i].size(); j++)
            std::cout << (char)(terrainMap[i][j].getTerrain());
        std::cout << '\n';
    }

    std::vector<std::vector<StructureTile>> structureMap = screen.getStructureMap();

    for(int i = 0; i < (int)structureMap.size(); i++) {
        for(int j = 0; j < (int)structureMap[i].size(); j++)
            std::cout << (char)(structureMap[i][j].getStructure());
        std::cout << '\n';
    }

    return 0;
}