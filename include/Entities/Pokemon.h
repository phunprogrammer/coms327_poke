#include "db_parse.h"
#include <string>
#include <vector>
#include "Config.h"

#ifndef POKEMON_H
#define POKEMON_H

enum Stat {
    HP,
    ATTACK,
    DEFENSE,
    SATTACK,
    SDEFENSE,
    SPEED
};

class Pokemon {
    private:
        int level;
        int exp;
        int maxExpIndex;
        int hp;

        bool shiny;
        int gender;
        std::vector<int> pokemonStats;
        std::vector<int> pokemonIVs;

        std::vector<int> movePP;
        std::vector<pokemon_move_db> learnedMoves;
        std::vector<pokemon_move_db> learnableMoves;

        pokemon_db pokemonData;
        pokemon_species_db pokemonSpecies;
        std::vector<pokemon_stats_db> pokemonBaseStats;
        std::vector<pokemon_types_db> pokemonTypes;

        int CalcStats();
        float CalcBaseDmg(move_db move, Pokemon enemy);
        float Effectiveness(move_db move, Pokemon enemy);
    public:
        Pokemon(pokemon_species_db pokemon, int level);
        Pokemon(coord_t screenCoord);
        ~Pokemon();
        std::string toString() const;

        const int& getLevel() const { return level; }
        const int& getExp() const { return exp; }
        const int& getMaxExp() const { return experience[maxExpIndex].experience; }
        const int& getHp() const { return hp; }
        const std::vector<pokemon_move_db>& getLearnedMoves() const { return learnedMoves; }
        const std::vector<pokemon_move_db>& getLearnableMoves() const { return learnableMoves; }
        const pokemon_db& getPokemonData() const { return pokemonData; }
        const pokemon_species_db& getPokemonSpecies() const { return pokemonSpecies; }
        const std::vector<pokemon_stats_db>& getPokemonBaseStats() const { return pokemonBaseStats; }
        const std::vector<int>& getPokemonStats() const { return pokemonStats; }
        const std::vector<pokemon_types_db>& getPokemonTypes() const { return pokemonTypes; }
        
        bool isFainted() { return hp <= 0; }

        void LoadData();
        int UpdateHP(int amount);
        int Heal();
        int IncrementExp(int amount);
        int Attack(move_db move, Pokemon& enemy);
};

#endif