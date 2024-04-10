#include "PokemonSpecies.h"
#include "Parsing.h"

#include <iostream>
#include <sstream>
#include <fstream>

PokemonSpecies::PokemonSpecies(std::string line) {
    std::istringstream stream(line);

    std::get<0>(data) = ParserUtil::getInt(stream);
    std::get<1>(data) = ParserUtil::getString(stream);
    std::get<2>(data) = ParserUtil::getInt(stream);
    std::get<3>(data) = ParserUtil::getInt(stream);
    std::get<4>(data) = ParserUtil::getInt(stream);
    std::get<5>(data) = ParserUtil::getInt(stream);
    std::get<6>(data) = ParserUtil::getInt(stream);
    std::get<7>(data) = ParserUtil::getInt(stream);
    std::get<8>(data) = ParserUtil::getInt(stream);
    std::get<9>(data) = ParserUtil::getInt(stream);
    std::get<10>(data) = ParserUtil::getInt(stream);
    std::get<11>(data) = ParserUtil::getInt(stream);
    std::get<12>(data) = ParserUtil::getInt(stream);
    std::get<13>(data) = ParserUtil::getInt(stream);
    std::get<14>(data) = ParserUtil::getInt(stream);
    std::get<15>(data) = ParserUtil::getInt(stream);
    std::get<16>(data) = ParserUtil::getInt(stream);
    std::get<17>(data) = ParserUtil::getInt(stream);
    std::get<18>(data) = ParserUtil::getInt(stream);
    std::get<19>(data) = ParserUtil::getInt(stream);
}

std::string PokemonSpecies::toString() const {
    std::string out;
    
    std::apply([&out](const auto&... args) {
        ((out += ParserUtil::toString(args) + ','), ...);
    }, data);
    out.pop_back();
    return out;
}

std::vector<PokemonSpecies*> PokemonSpecies::parse(const std::string& filename) {
    std::string filepath = "/share/cs327/pokedex/pokedex/data/csv/" + filename + ".csv";
    std::string filepath2 = std::string(std::getenv("HOME")) + "/.poke327/pokedex/pokedex/data/csv/" + filename + ".csv";

    std::vector<PokemonSpecies*> data;
    std::ifstream file = std::ifstream(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filepath << std::endl;

        file = std::ifstream(filepath2);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filepath2 << std::endl;
            return data;
        }
    }

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        data.push_back(new PokemonSpecies(line));
    }

    file.close();

    return data;
}