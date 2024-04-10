#include "Parsing.h"
#include <string>
#include <tuple>

#ifndef POKEMONSPECIES_H
#define POKEMONSPECIES_H

class PokemonSpecies : public Data {
    private:
        std::tuple
        <
            int,
            std::string,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int,
            int
        > data;

    public:
        PokemonSpecies(std::string line);

        auto getData() const { return data; }

        std::string toString() const;

        static std::vector<PokemonSpecies*> parse(const std::string& filename);
};

#endif