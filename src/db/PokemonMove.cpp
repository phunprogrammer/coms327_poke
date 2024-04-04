#include "PokemonMove.h"
#include "Parsing.h"
#include <iostream>
#include <sstream>
#include <fstream>

PokemonMove::PokemonMove(std::string line) {
    std::istringstream stream(line);

    std::get<0>(data) = ParserUtil::getInt(stream);
    std::get<1>(data) = ParserUtil::getInt(stream);
    std::get<2>(data) = ParserUtil::getInt(stream);
    std::get<3>(data) = ParserUtil::getInt(stream);
    std::get<4>(data) = ParserUtil::getInt(stream);
    std::get<5>(data) = ParserUtil::getInt(stream);
}

std::string PokemonMove::toString() const {
    std::string out;
    
    std::apply([&out](const auto&... args) {
        ((out += ParserUtil::toString(args) + ','), ...);
    }, data);
    out.pop_back();
    return out;
}

std::vector<Data*> PokemonMove::parse(const std::string& filename) {
    std::vector<Data*> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        data.push_back(new PokemonMove(line));
    }

    file.close();

    return data;
}