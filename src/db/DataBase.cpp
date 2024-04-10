#include "DataBase.h"

DataBase::DataBase() {
    pokemon = Pokemon::parse("pokemon");
    moves = Move::parse("moves");
    pokemonMoves = PokemonMove::parse("pokemon_moves");
    pokemonSpecies = PokemonSpecies::parse("pokemon_species");
    experience = Experience::parse("experience");
    typeNames = TypeName::parse("type_names");
    pokemonStats = PokemonStat::parse("pokemon_stats");
    stats = Stat::parse("stats");
    pokemonTypes = PokemonType::parse("pokemon_types");
}

DataBase::~DataBase() {
    for (auto& item : experience) { delete item; }
    experience.clear();

    for (auto& item : pokemon) { delete item; }
    pokemon.clear();

    for (auto& item : moves) { delete item; }
    moves.clear();

    for (auto& item : stats) { delete item; }
    stats.clear();

    for (auto& item : typeNames) { delete item; }
    typeNames.clear();

    for (auto& item : pokemonMoves) { delete item; }
    pokemonMoves.clear();

    for (auto& item : pokemonSpecies) { delete item; }
    pokemonSpecies.clear();

    for (auto& item : pokemonStats) { delete item; }
    pokemonStats.clear();

    for (auto& item : pokemonTypes) { delete item; }
    pokemonTypes.clear();
}