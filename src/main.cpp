#include "Config.h"
#include "Screen.h"
#include "AbstractTiles.h"
#include "PCTile.h"
#include <iostream>
#include <ncurses.h>

int main() {
    Initialize();
    

    int seed;
    waves_t waves = Screen::GetWaves(&seed);
    coord_t coord = { 200, 200 };


    Screen screen = Screen(waves, coord, new PCTile(screen, {0, 0}));

    std::vector<std::vector<TerrainTile>> terrainMap = screen.getTerrainMap();
    std::vector<std::vector<StructureTile>> structureMap = screen.getStructureMap();

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    
    for (int i = 0; i < WIDTH; ++i) {
        for (int j = 0; j < LENGTH; ++j) {
            if(structureMap[i][j].getStructure() != NULL_STRUCT)
                mvaddch(i, j, (char)structureMap[i][j].getStructure());
            else
                mvaddch(i, j, (char)terrainMap[i][j].getTerrain());
        }
    }

    MapVector entities = screen.getEntities();
    for(int i = 0; i < (int)entities.size(); i++) {
        auto entity = entities[i];
        mvaddch(entity->getCoord().y, entity->getCoord().x, (char)entity->getEntity());
    }

    refresh(); // Refresh the screen to display changes
    getch(); // Wait for user input before exiting
    endwin(); // End ncurses mode

    return 0;
}