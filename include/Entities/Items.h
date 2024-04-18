#include <string>
#include <map>
#include <vector>

#ifndef ITEMS_H
#define ITEMS_H

enum ItemEnum {
    POTION,
    REVIVE,
    POKEBALL
};

const std::map<ItemEnum, std::string> ITEM_STRINGS ({
    { ItemEnum::POTION, "Potion" },
    { ItemEnum::REVIVE, "Revive" },
    { ItemEnum::POKEBALL, "Pokeball" }
});

const std::vector<ItemEnum> ITEMS({ ItemEnum::POTION, ItemEnum::REVIVE, ItemEnum::POKEBALL });

class Pokemon;
class PCTile;

int UsePotion(Pokemon& ally);
int UseRevive(Pokemon& ally);
int UsePokeball(PCTile* pc, Pokemon& enemy);

#endif