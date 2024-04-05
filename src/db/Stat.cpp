#include "Stat.h"
#include "Parsing.h"
#include <iostream>
#include <sstream>
#include <fstream>

Stat::Stat(std::string line) {
    std::istringstream stream(line);

    std::get<0>(data) = ParserUtil::getInt(stream);
    std::get<1>(data) = ParserUtil::getInt(stream);
    std::get<2>(data) = ParserUtil::getString(stream);
    std::get<3>(data) = ParserUtil::getInt(stream);
    std::get<4>(data) = ParserUtil::getInt(stream);
}

std::string Stat::toString() const {
    std::string out;
    
    std::apply([&out](const auto&... args) {
        ((out += ParserUtil::toString(args) + ','), ...);
    }, data);
    out.pop_back();
    return out;
}

std::vector<Data*> Stat::parse(const std::string& filename) {
    std::string filepath = "/share/coms327/pokedex/pokedex/data/csv/" + filename + ".csv";
    std::string filepath2 = std::string(std::getenv("HOME")) + "/.poke327/pokedex/pokedex/data/csv/" + filename + ".csv";

    std::vector<Data*> data;
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
        data.push_back(new Stat(line));
    }

    file.close();

    return data;
}