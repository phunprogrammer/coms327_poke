#include <fstream>
#include <sstream>
#include <iostream>
#include <climits>

#include "Parsing.h"
#include "Pokemon.h"

template<typename T>
Parser<T>::Parser(const std::string& filename) : filename(filename) {}

template<typename T>
std::vector<T*> Parser<T>::parse() {
    std::vector<T*> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        data.push_back(new T(line));
    }

    file.close();

    return data;
}

template class Parser<Pokemon>;

bool ParserUtil::isNumber(const std::string& string) {
    std::string::const_iterator it = string.begin();
    while (it != string.end() && std::isdigit(*it)) ++it;
    return !string.empty() && it == string.end();
}

int ParserUtil::getInt(std::istringstream& stream) {
    std::string out;
    std::getline(stream, out, ',');
    return isNumber(out) ? std::stoi(out) : INT_MAX;
}

std::string ParserUtil::getString(std::istringstream& stream) {
    std::string out;
    std::getline(stream, out, ',');
    return out;
}

std::string ParserUtil::toString(const int& num) {
    return num == INT_MAX ? "" : std::to_string(num);
}

std::string ParserUtil::toString(const std::string& string) {
    return string;
}