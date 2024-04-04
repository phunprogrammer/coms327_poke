#include <fstream>
#include <sstream>
#include <iostream>
#include <climits>

#include "Parsing.h"
#include "Pokemon.h"
#include "Move.h"
#include "PokemonMove.h"

bool ParserUtil::isNumber(const std::string& string) {
    if (string.empty())
        return false;
    std::string::const_iterator it = string.begin();
    if (*it == '-')
        ++it;
    while (it != string.end() && std::isdigit(*it))
        ++it;
    return it == string.end();
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