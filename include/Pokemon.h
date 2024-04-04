#include <iostream>
#include <vector>
#include <string>

#ifndef POKEMON_H
#define POKEMON_H

class Pokemon {
    private:
        int id;
        std::string identifier;
        int species_id;
        int height;
        int weight;
        int base_experience;
        int order;
        int is_default;

    public:
        Pokemon(std::vector<std::string> tokens)
            : id(stoi(tokens[0])), identifier(tokens[1]), species_id(stoi(tokens[2])), height(stoi(tokens[3])),
            weight(stoi(tokens[4])), base_experience(stoi(tokens[5])), order(stoi(tokens[6])), is_default(stoi(tokens[7])) {}

        int getId() const { return id; }
        const std::string& getIdentifier() const { return identifier; }
        int getSpeciesId() const { return species_id; }
        int getHeight() const { return height; }
        int getWeight() const { return weight; }
        int getBaseExperience() const { return base_experience; }
        int getOrder() const { return order; }
        int isDefault() const { return is_default; }

        std::string toString() const;
};

#endif