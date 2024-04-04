#include "Parsing.h"
#include <string>
#include <tuple>

#ifndef POKEMONMOVE_H
#define POKEMONMOVE_H

class PokemonMove : public Data {
    private:
        std::tuple
        <
            int, /*pokemon_id*/
            int, /*version_group_id*/
            int, /*move_id*/
            int, /*pokemon_move_method_id*/
            int, /*level*/
            int /*order*/
        > data;

    public:
        PokemonMove(std::string line);

        auto getData() const { return data; }

        std::string toString() const;

        static std::vector<Data*> parse(const std::string& filename);
};

#endif