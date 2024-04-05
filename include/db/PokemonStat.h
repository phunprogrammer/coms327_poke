#include "Parsing.h"
#include <string>
#include <tuple>

#ifndef POKEMONSTAT_H
#define POKEMONSTAT_H

class PokemonStat : public Data {
    private:
        std::tuple
        <
            int,
            int, 
            int,
            int
        > data;

    public:
        PokemonStat(std::string line);

        auto getData() const { return data; }

        std::string toString() const;

        static std::vector<Data*> parse(const std::string& filename);
};

#endif