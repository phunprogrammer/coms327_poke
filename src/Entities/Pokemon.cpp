#include "Pokemon.h"
#include <iostream>
#include <sstream>
#include <algorithm> 

Pokemon::Pokemon(pokemon_db pokemon, int level) : level(level), pokemon(pokemon) {
    pokemonSpecies = pokemon_species[pokemon.species_id];

    int i;
    for(i = 1; i < pokemon_stats_size; i++)
        if(pokemon_stats[i].pokemon_id == pokemon.species_id)
            break;

    for(; i < pokemon_stats_size; i++) {
        if(pokemon_stats[i].pokemon_id != pokemon.species_id)
            break;
        pokemonStats.push_back(pokemon_stats[i]);
    }

    for(i = 1; i < pokemon_types_size; i++)
        if(pokemon_types[i].pokemon_id == pokemon.species_id)
            break;

    for(; i < pokemon_types_size; i++) {
        if(pokemon_types[i].pokemon_id != pokemon.species_id)
            break;
        pokemonTypes.push_back(pokemon_types[i]);
    }

    for(i = 1; i < pokemon_moves_size; i++)
        if(pokemon_moves[i].pokemon_id == pokemon.species_id)
            break;

    for(; i < pokemon_moves_size; i++) {
        if(pokemon_moves[i].pokemon_id != pokemon.species_id)
            break;

        if(pokemon_moves[i].pokemon_move_method_id == 1 && !std::count(learnableMoves.begin(), learnableMoves.end(), pokemon_moves[i]))
            learnableMoves.push_back(pokemon_moves[i]);
    }
    int minMoveLevel = learnableMoves[0].level;

    for(int i = 1; i < (int)learnableMoves.size(); i++) {
        int current = learnableMoves[i].level;
        if(learnableMoves[i].level < minMoveLevel)
            minMoveLevel = learnableMoves[i].level;
    }

    level = std::max(minMoveLevel, this->level);

    std::vector<int> validMoves;
    for(int i = 0; i < (int)learnableMoves.size(); i++)
        if(learnableMoves[i].level <= this->level)
            validMoves.push_back(i);

    while(validMoves.size() > 0 && learnedMoves.size() < 4) {
        int index = rand() % validMoves.size();
        learnedMoves.push_back(learnableMoves[validMoves[index]]);
        validMoves.erase(validMoves.begin() + index);
    }
};

Pokemon::~Pokemon() {}

std::string Pokemon::toString() const {
    std::ostringstream oss;
    oss << "Pokemon: " << pokemon.identifier << std::endl;
    oss << "Species: " << pokemonSpecies.identifier << std::endl;
    oss << "Level: " << level << std::endl;
    //oss << "Experience: " << experience << std::endl;

    oss << "Stats:" << std::endl;
    for (const auto& stat : pokemonStats) {
        oss << " - " << std::string(stats[stat.stat_id].identifier) << ": " << stat.base_stat << std::endl;
    }

    oss << "Types:" << std::endl;
    for (const auto& type : pokemonTypes) {
        oss << " - " << std::string(types[type.type_id]) << std::endl;
    }

    oss << "Learnable Moves:" << std::endl;
    for (const auto& move : learnableMoves) {
        oss << " - " << std::string(moves[move.move_id].identifier) << std::endl;
    }

    oss << "Learned Moves:" << std::endl;
    for (const auto& move : learnedMoves) {
        oss << " - " << std::string(moves[move.move_id].identifier) << std::endl;
    }

    return oss.str();
}