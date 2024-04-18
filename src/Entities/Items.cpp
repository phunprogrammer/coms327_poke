#include "Items.h"
#include "Pokemon.h"
#include "PCTile.h"

int UsePotion(Pokemon& ally) {
    if(ally.getHp() == ally.getPokemonStats()[Stat::HP]) return 0;
    ally.UpdateHP(20);
    return 1;
}

int UseRevive(Pokemon& ally) {
    if(!ally.isFainted()) return 0;
    ally.UpdateHP(ally.getPokemonStats()[Stat::HP] / 2);
    return 1;
}

int UsePokeball(PCTile* pc, Pokemon& enemy) {
    return pc->addToParty(enemy);
}