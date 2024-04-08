#include "SentryTile.h"
#include "Screen.h"

SentryTile::SentryTile(Screen& screen) : 
    NPCTile(Entity::SENTRY, screen) {
}

int SentryTile::queueMove() {

}