#include "AbstractTiles.h"
#include "db_parse.h"
#include <string>

#ifndef POKEMON_H
#define POKEMON_H

class Pokemon {
    private:
        int level;
        int experience;

        std::vector<pokemon_move_db> learnedMoves;
        std::vector<pokemon_move_db> learnableMoves;

        pokemon_db pokemon;
        pokemon_species_db pokemonSpecies;
        std::vector<pokemon_stats_db> pokemonStats;
        std::vector<pokemon_types_db> pokemonTypes;
    public:
        Pokemon(pokemon_db pokemon, int level);
        std::string toString() const;
};

#endif