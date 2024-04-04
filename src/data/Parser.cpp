#include "Parser.h"
#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <climits>

#include "Pokemon.h"
#include "Move.h"

template<typename T>
Parser<T>::Parser(const std::string& filename) : filename(filename) {}

template<typename T>
std::vector<T> Parser<T>::parse() {
    std::vector<T> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    std::string firstLine;
    if (!std::getline(file, firstLine)) {
        std::cerr << "Error reading first line from file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ',')) {
            if (token.empty()) {
                tokens.push_back(std::to_string(MAX)); 
            } else {
                tokens.push_back(token);
            }
        }

        try {
            T obj(tokens);
            data.push_back(obj);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error creating object: " << e.what() << std::endl;
        }
    }

    file.close();

    return data;
}

template class Parser<Pokemon>;
template class Parser<Move>;