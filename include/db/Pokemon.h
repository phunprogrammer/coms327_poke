#include "Parsing.h"

#include <string>
#include <tuple>

#ifndef POKEMON_H
#define POKEMON_H

class Pokemon : public Data {
    private:
        std::tuple
        <
            int, /*id*/
            std::string, /*identifier*/
            int, /*species_id*/
            int, /*height*/
            int, /*weight*/
            int, /*base_experience*/
            int, /*order*/
            int /*is_default*/
        > data;

    public:
        Pokemon(std::string line);

        auto getData() const { return data; }

        std::string toString() const;

        static std::vector<Data*> parse(const std::string& filename);
};

#endif