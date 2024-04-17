#include <string>
#include <map>

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

class Pokemon;
class PCTile;

class Item {
    protected:
        PCTile* pc;
        Item(ItemEnum type, PCTile* pc);
        ItemEnum item;
    public:
        virtual int use(Pokemon& ally, Pokemon& enemy, bool wild) = 0;
        virtual int getAmount() = 0;
        virtual ItemEnum getItem() = 0;
};

class Potion : public Item {
    public:
        Potion(PCTile* pc);
        int use(Pokemon& ally, Pokemon& enemy, bool wild);
        const ItemEnum getItem() const { return item; }
};

class Revive : public Item {
    public:
        Revive(PCTile* pc);
        int use(Pokemon& ally, Pokemon& enemy, bool wild);
        const ItemEnum getItem() const { return item; }
};

class Pokeball : public Item {
    public:
        Pokeball(PCTile* pc);
        int use(Pokemon& ally, Pokemon& enemy, bool wild);
        const ItemEnum getItem() const { return item; }
};

#endif