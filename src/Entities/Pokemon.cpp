#include "Pokemon.h"
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <Config.h>
#include <cmath>

Pokemon::Pokemon(pokemon_db pokemon, int level) : level(level), pokemonData(pokemon) {
    LoadData();
}

Pokemon::Pokemon(coord_t screenCoord) {
    pokemonData = pokemon[rand() % (pokemon_db_size - 1) + 1];

    int dist = abs((int)(screenCoord.x) - MAXSIZE / 2) + abs((int)(screenCoord.y) - MAXSIZE / 2);
    int minLevel, maxLevel;

    if (dist <= 200) {
        minLevel = 1;
        maxLevel = std::max(1, (dist + 1) / 2);
    } else {
        minLevel = (dist - 200) / 2;
        maxLevel = 100;
    }

    level = rand() % (maxLevel - minLevel + 1) + minLevel;

    LoadData();
}

void Pokemon::LoadData() {
    pokemonSpecies = pokemon_species[pokemonData.species_id];

    int i;
    for(i = 1; i < pokemon_stats_size; i++)
        if(pokemon_stats[i].pokemon_id == pokemonData.species_id)
            break;

    for(; i < pokemon_stats_size; i++) {
        if(pokemon_stats[i].pokemon_id != pokemonData.species_id)
            break;
        pokemonBaseStats.push_back(pokemon_stats[i]);
    }

    pokemonIVs.push_back(rand() % 16);
    int value = std::floor(((pokemonBaseStats[0].base_stat + pokemonIVs[0]) * 2 * level) / 100) + level + 10;
    pokemonStats.push_back(value);

    for(int j = 1; j < (int)pokemonBaseStats.size(); j++) {
        pokemonIVs.push_back(rand() % 16);
        value = std::floor(((pokemonBaseStats[j].base_stat + pokemonIVs[j]) * 2 * level) / 100) + 5;
        pokemonStats.push_back(value);
    }
        

    for(i = 1; i < pokemon_types_size; i++)
        if(pokemon_types[i].pokemon_id == pokemonData.species_id)
            break;

    for(; i < pokemon_types_size; i++) {
        if(pokemon_types[i].pokemon_id != pokemonData.species_id)
            break;
        pokemonTypes.push_back(pokemon_types[i]);
    }

    for(i = 1; i < pokemon_moves_size; i++)
        if(pokemon_moves[i].pokemon_id == pokemonData.species_id)
            break;

    for(; i < pokemon_moves_size; i++) {
        if(pokemon_moves[i].pokemon_id != pokemonData.species_id)
            break;

        if(pokemon_moves[i].pokemon_move_method_id == 1 && !std::count(learnableMoves.begin(), learnableMoves.end(), pokemon_moves[i]))
            learnableMoves.push_back(pokemon_moves[i]);
    }
    int minMoveLevel = learnableMoves[0].level;

    for(int i = 1; i < (int)learnableMoves.size(); i++)
        if(learnableMoves[i].level < minMoveLevel)
            minMoveLevel = learnableMoves[i].level;

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

    if(rand() % 8192 == 0)
        shiny = true;
    else
        shiny = false;

    if(rand() % 2 == 0)
        gender = 0;
    else
        gender = 1;
}

Pokemon::~Pokemon() {}

std::string Pokemon::toString() const {
    std::ostringstream oss;
    oss << "Pokemon: " << pokemonData.identifier << std::endl;
    oss << "Level: " << level;
    oss << "  Shiny: " << shiny;
    oss << "  Gender: " << (gender == 0 ? "male" : "female") << std::endl;

    oss << "Stats:" << std::endl;
    for (const auto& stat : pokemonBaseStats) {
        oss << " - " << std::string(stats[stat.stat_id].identifier) << ": " << pokemonStats[stat.stat_id] << std::endl;
    }

    oss << "Types:" << std::endl;
    for (const auto& type : pokemonTypes) {
        oss << " - " << std::string(types[type.type_id]);
    }
    oss << std::endl;

    // oss << "Learnable Moves:" << std::endl;
    // for (const auto& move : learnableMoves) {
    //     oss << " - " << std::string(moves[move.move_id].identifier) << std::endl;
    // }

    oss << "Moves:" << std::endl;
    for (const auto& move : learnedMoves) {
        oss << " - " << std::string(moves[move.move_id].identifier) << std::endl;
    }

    return oss.str();
}