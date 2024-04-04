#include <vector>
#include <string>

#ifndef PARSER_H
#define PARSER_H

template<typename T>
class Parser {
    private:
        std::string filename;
    public:
        Parser(const std::string& filename);
        std::vector<T> parse();
};

#endif