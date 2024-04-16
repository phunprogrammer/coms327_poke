#include "Pokemon.h"
#include <iostream>
#include <sstream>
#include <algorithm> 
#include <Config.h>
#include <cmath>
#include <climits>

Pokemon::Pokemon(pokemon_species_db pokemon, int level) : level(level), pokemonSpecies(pokemon) {
    LoadData();
}

Pokemon::Pokemon(coord_t screenCoord) {
    pokemonSpecies = pokemon_species[rand() % (pokemon_species_size - 1) + 1];

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
    pokemonData = pokemon[pokemonSpecies.id];

    int i;
    for(i = 1; i < pokemon_stats_size; i++)
        if(pokemon_stats[i].pokemon_id == pokemonData.species_id)
            break;

    for(; i < pokemon_stats_size; i++) {
        if(pokemon_stats[i].pokemon_id != pokemonData.species_id)
            break;
        pokemonBaseStats.push_back(pokemon_stats[i]);
    }

    for(i = 0; i < (int)pokemonBaseStats.size(); i++) {
        int randInt = rand() % 16;
        pokemonIVs.push_back(randInt);
    }

    CalcStats();
    hp = pokemonStats[0];

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

    if(level == 100) {
        maxExpIndex = 1;
    }
    else {
        for(i = 1; i < experience_size; i++) {
            if(experience[i].growth_rate_id == this->pokemonSpecies.growth_rate_id && experience[i].level == this->level) {
                maxExpIndex = ++i;
                break;
            }
        }
    }
}

Pokemon::~Pokemon() {}

std::string Pokemon::toString() const {
    std::ostringstream oss;
    oss << "Pokemon: " << pokemonData.identifier << std::endl;
    oss << "Exp: " << exp << "/" << getMaxExp();
    oss << "  Health: " << hp << "/" << pokemonStats[0] << std::endl;
    oss << "Level: " << level;
    oss << "  Shiny: " << shiny;
    oss << "  Gender: " << (gender == 0 ? "male" : "female") << std::endl;

    oss << "Stats, IV:" << std::endl;
    for (const auto& stat : pokemonBaseStats) {
        oss << " - " << std::string(stats[stat.stat_id].identifier) << ": " << pokemonStats[stat.stat_id - 1] << ", " << pokemonIVs[stat.stat_id - 1] << std::endl;
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

int Pokemon::UpdateHP(int amount) {
    if(amount == 0 || (amount < 0 && hp == 0) || (amount > 0 && hp == pokemonStats[0])) return 0;
    hp = std::max(std::min(pokemonStats[0], hp + amount), 0);
    return 1;
}
int Pokemon::IncrementExp(int amount) {
    if(level == 100 || amount <= 0) return 0;

    int used = amount;
    
    while(used > 0 && level < 100) {
        if(exp + used >= experience[maxExpIndex].experience) {
            used -= experience[maxExpIndex].experience - exp;
            exp = 0;
            maxExpIndex++;
            level++;
        }
        else {
            exp += used;
            used = 0;
        }
    }

    CalcStats();
    return 1;
}

int Pokemon::CalcStats() {
    pokemonStats.clear();
    int value = std::floor(((pokemonBaseStats[0].base_stat + pokemonIVs[0]) * 2 * level) / 100.0) + level + 10;
    pokemonStats.push_back(value);

    for(int j = 1; j < (int)pokemonBaseStats.size(); j++) {
        value = std::floor(((pokemonBaseStats[j].base_stat + pokemonIVs[j]) * 2 * level) / 100.0) + 5;
        pokemonStats.push_back(value);
    }

    return 1;
}

float Pokemon::CalcBaseDmg(move_db move, Pokemon enemy) {
    float baseDmg = ((2 * this->level) / 5.0 + 2) * (float)move.power * ((float)pokemonStats[Stat::ATTACK] / (float)enemy.getPokemonStats()[Stat::DEFENSE]);
    return (baseDmg / 50.0) + 2;
}

float Pokemon::Effectiveness(move_db move, Pokemon enemy) {
    int start = ((move.type_id - 1) * 18) + 1;
    float efficacy = type_efficacy[start + enemy.getPokemonTypes()[0].type_id - 1].damage_factor / 100.0;

    if((int)enemy.getPokemonTypes().size() > 1)
        efficacy *= type_efficacy[start + enemy.getPokemonTypes()[1].type_id - 1].damage_factor / 100.0;

    return efficacy;
}

int Pokemon::Attack(move_db move, Pokemon& enemy) {
    if(move.power == INT_MAX || rand() % 100 + 1 > move.accuracy)
        return 0;

    float baseDmg = CalcBaseDmg(move, enemy);
    float randMultiplier = (rand() % 16 + 85) / 100.0;
    float stabMultiplier = move.type_id == pokemonTypes[0].type_id || ((int)pokemonTypes.size() > 1 && move.type_id == pokemonTypes[1].type_id) ? 1.5 : 1;
    float typeMultiplier = Effectiveness(move, enemy);
    float critMultiplier = ((rand() % 256) < pokemonBaseStats[Stat::SPEED].base_stat / 2.0) ? 1.5 : 1;

    int totalDmg = baseDmg * critMultiplier * randMultiplier  * stabMultiplier * typeMultiplier;

    enemy.UpdateHP(totalDmg * -1);

    return 1;
}