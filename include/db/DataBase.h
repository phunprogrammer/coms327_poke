#include "Parsing.h"
#include <string>
#include <tuple>
#include "Pokemon.h"
#include "Experience.h"
#include "Move.h"
#include "PokemonMove.h"
#include "PokemonSpecies.h"
#include "PokemonStat.h"
#include "PokemonType.h"
#include "Stat.h"
#include "TypeName.h"

#ifndef DATABASE_H
#define DATABASE_H

class DataBase {
    public:
        std::vector<Experience*> experience;
        std::vector<Pokemon*> pokemon;
        std::vector<Move*> moves;
        std::vector<Stat*> stats;
        std::vector<TypeName*> typeNames;
        std::vector<PokemonMove*> pokemonMoves;
        std::vector<PokemonSpecies*> pokemonSpecies;
        std::vector<PokemonStat*> pokemonStats;
        std::vector<PokemonType*> pokemonTypes;
    public:
        DataBase();
        ~DataBase();

        const std::vector<Experience*>& getExperience() const { return experience; }
        const std::vector<Pokemon*>& getPokemon() const { return pokemon; }
        const std::vector<Move*>& getMoves() const { return moves; }
        const std::vector<Stat*>& getStats() const { return stats; }
        const std::vector<TypeName*>& getTypeNames() const { return typeNames; }
        const std::vector<PokemonMove*>& getPokemonMoves() const { return pokemonMoves; }
        const std::vector<PokemonSpecies*>& getPokemonSpecies() const { return pokemonSpecies; }
        const std::vector<PokemonStat*>& getPokemonStats() const { return pokemonStats; }
        const std::vector<PokemonType*>& getPokemonTypes() const { return pokemonTypes; }
};

#endif