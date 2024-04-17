#include "Items.h"
#include "Pokemon.h"
#include "PCTile.h"

Item::Item(ItemEnum type, PCTile* pc) : item(type), pc(pc) {}

Potion::Potion(PCTile* pc) : Item(ItemEnum::POTION, pc) {}

int Potion::use(Pokemon& ally, Pokemon& enemy, bool wild) {
    if(ally.getHp() == ally.getPokemonStats()[Stat::HP]) return 0;
    ally.UpdateHP(20);
    return 1;
}

Revive::Revive(PCTile* pc) : Item(ItemEnum::REVIVE, pc) {}

int Revive::use(Pokemon& ally, Pokemon& enemy, bool wild) {
    if(!ally.isFainted()) return 0;
    ally.UpdateHP(ally.getPokemonStats()[Stat::HP] / 2);
    return 1;
}

Pokeball::Pokeball(PCTile* pc) : Item(ItemEnum::POKEBALL, pc) {}

int Pokeball::use(Pokemon& ally, Pokemon& enemy, bool wild) {
    if(!wild) return 0;
    pc->addToParty(enemy);
    return 1;
}