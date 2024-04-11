#include "db_parse.h"
#include <string>
#include <vector>

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
        ~Pokemon();
        std::string toString() const;

        const int& getLevel() const { return level; }
        const int& getExperience() const { return experience; }
        const std::vector<pokemon_move_db>& getLearnedMoves() const { return learnedMoves; }
        const std::vector<pokemon_move_db>& getLearnableMoves() const { return learnableMoves; }
        const pokemon_db& getPokemon() const { return pokemon; }
        const pokemon_species_db& getPokemonSpecies() const { return pokemonSpecies; }
        const std::vector<pokemon_stats_db>& getPokemonStats() const { return pokemonStats; }
        const std::vector<pokemon_types_db>& getPokemonTypes() const { return pokemonTypes; }
};

#endif