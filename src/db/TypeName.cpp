#include "TypeName.h"
#include "Parsing.h"
#include <iostream>
#include <sstream>
#include <fstream>

TypeName::TypeName(std::string line) {
    std::istringstream stream(line);

    std::get<0>(data) = ParserUtil::getInt(stream);
    std::get<1>(data) = ParserUtil::getInt(stream);
    std::get<2>(data) = ParserUtil::getString(stream);
}

std::string TypeName::toString() const {
    std::string out;
    
    std::apply([&out](const auto&... args) {
        ((out += ParserUtil::toString(args) + ','), ...);
    }, data);
    out.pop_back();
    return out;
}

std::vector<TypeName*> TypeName::parse(const std::string& filename) {
    std::string filepath = "/share/cs327/pokedex/pokedex/data/csv/" + filename + ".csv";
    std::string filepath2 = std::string(std::getenv("HOME")) + "/.poke327/pokedex/pokedex/data/csv/" + filename + ".csv";

    std::vector<TypeName*> data;
    std::ifstream file = std::ifstream(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filepath << std::endl;

        file = std::ifstream(filepath2);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filepath2 << std::endl;
            return data;
        }
    }

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        data.push_back(new TypeName(line));
    }

    file.close();

    return data;
}