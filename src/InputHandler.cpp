#include "InputHandler.h"
#include "Screen.h"
#include "CursesHandler.h"
#include "PCTile.h"
#include "AbstractTiles.h"
#include "Tiles.h"
#include <cstdlib>

InputHandler::InputHandler(Screen *screen) : mapGrid(MAXSIZE + 1, std::vector<MapVector<coord_t, EntityTile*>>(MAXSIZE + 1)), screen(screen) {
    mapGrid[screen->getCoord().y][screen->getCoord().x] = screen->getEntities();
}

InputHandler::~InputHandler() {
    for(int y = 0; y < MAXSIZE; y++)
        for(int x = 0; x < MAXSIZE; x++)
            for(int i = 1; i < (int)mapGrid[y][x].size(); i++)
                delete mapGrid[y][x][i];

    delete screen;
}

int InputHandler::HandleInput(char input) {
    switch(input) {
        case 'q':
        case '7':
        case 'w':
        case '8':
        case 'e':
        case '9':
        case 'a':
        case '4':
        case 'd':
        case '6':
        case 'z':
        case '1':
        case 'x':
        case '2':
        case 'c':
        case '3':
        case 's':
        case '5':
            return MovePC(input);
        case 't':
            return screen->getCursesHandler().ListTrainers();
        case 'f':
            return Fly();
        case 10:
            EnterPokeMart();
            EnterPokeCenter();
            return 1;
    }

    return 0;
}

int InputHandler::MovePC(char input) {
    PCTile* pc = (PCTile*)(screen->getEntities()[0]);

    switch(input) {
        case 'q':
        case '7':
            pc->setDirection({ -1, -1 });
            break;
        case 'w':
        case '8':
            pc->setDirection({ 0, -1 });
            break;
        case 'e':
        case '9':
            pc->setDirection({ 1, -1 });
            break;
        case 'a':
        case '4':
            pc->setDirection({ -1, 0 });
            break;
        case 'd':
        case '6':
            pc->setDirection({ 1, 0 });
            break;
        case 'z':
        case '1':
            pc->setDirection({ -1, 1 });
            break;
        case 'x':
        case '2':
            pc->setDirection({ 0, 1 });
            break;
        case 'c':
        case '3':
            pc->setDirection({ 1, 1 });
            break;
        case 's':
        case '5':
            pc->setDirection({ 0, 0 });
            break;
    }

    int out;
    coord_t move = { pc->getCoord().x + pc->getDirection().x, pc->getCoord().y + pc->getDirection().y };

    if(move.x < 0)
        out = MoveScreen('w');
    else if(move.x >= LENGTH)
        out = MoveScreen('e');
    else if (move.y < 0)
        out = MoveScreen('n');
    else if (move.y >= WIDTH)
        out = MoveScreen('s');
    else
        out = pc->move();

    return out;
}

int InputHandler::MoveScreen(char compass) {
    PCTile* pc = (PCTile*)(screen->getEntities()[0]);
    coord_t oldCoords = screen->getCoord();
    screen->getEntities().remove(pc->getCoord());
    mapGrid[oldCoords.y][oldCoords.x] = screen->getEntities();
    delete screen;

    coord_t screenMove;
    coord_t pcMove;

    switch(compass) {
        case 'w':
            screenMove = { oldCoords.x - 1, oldCoords.y };
            pcMove = { LENGTH - 1, pc->getCoord().y };
            break;
        case 'e':
            screenMove = { oldCoords.x + 1, oldCoords.y };
            pcMove = { 0, pc->getCoord().y };
            break;
        case 'n':
            screenMove = { oldCoords.x, oldCoords.y - 1 };
            pcMove = { pc->getCoord().x, WIDTH - 1 };
            break;
        case 's':
            screenMove = { oldCoords.x, oldCoords.y + 1 };
            pcMove = { pc->getCoord().x, 0 };
            break;
    }

    Screen* newScreen;

    if(mapGrid[screenMove.y][screenMove.x].size() != 0)
        newScreen = new Screen(waves, screenMove, pc, pcMove, mapGrid[screenMove.y][screenMove.x]);
    else
        newScreen = new Screen(waves, screenMove, pc, pcMove);

    this->screen = newScreen;
    return 1;
}

int InputHandler::MoveScreen(coord_t coord) {
    PCTile* pc = (PCTile*)(screen->getEntities()[0]);
    coord_t oldCoords = screen->getCoord();
    screen->getEntities().remove(pc->getCoord());
    mapGrid[oldCoords.y][oldCoords.x] = screen->getEntities();
    delete screen;

    Screen* newScreen;

    if(mapGrid[coord.y][coord.x].size() != 0)
        newScreen = new Screen(waves, coord, pc, mapGrid[coord.y][coord.x]);
    else
        newScreen = new Screen(waves, coord, pc);

    this->screen = newScreen;
    return 1;
}

int InputHandler::Fly() {
    echo();
    curs_set(1);
    move(TERM_WIDTH, 0);

    int x, y;
    if(scanw("%d %d", &x, &y) == 2) {
        int screenX = std::max(std::min(MIDDLEX + x, MAXSIZE), MINSIZE);
        int screenY = std::max(std::min(MIDDLEY + y, MAXSIZE), MINSIZE);
        MoveScreen({ screenX, screenY });
    }

    move(TERM_WIDTH, 0);
    clrtoeol();

    noecho();
    curs_set(0);

    return 1;
}

int InputHandler::EnterPokeCenter() {
    PCTile* pc = (PCTile*)(screen->getEntities()[0]);
    if((*screen)[pc->getCoord()] != Structure::PCNTR) return 0;
    pc->healParty();
    screen->getCursesHandler().HealPoke();
    return 1;
}

int InputHandler::EnterPokeMart() {
    PCTile* pc = (PCTile*)(screen->getEntities()[0]);
    if((*screen)[pc->getCoord()] != Structure::PMART) return 0;
    pc->healParty();
    screen->getCursesHandler().HealPoke();
    return 1;
}