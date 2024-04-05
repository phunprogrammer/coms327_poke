#include "Parsing.h"
#include <string>
#include <tuple>

#ifndef POKEMONTYPE_H
#define POKEMONTYPE_H

class PokemonType : public Data {
    private:
        std::tuple
        <
            int,
            int, 
            int
        > data;

    public:
        PokemonType(std::string line);

        auto getData() const { return data; }

        std::string toString() const;

        static std::vector<Data*> parse(const std::string& filename);
};

#endif