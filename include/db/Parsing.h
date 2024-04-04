#include <vector>
#include <string>

#ifndef PARSING_H
#define PARSING_H

template<typename T>
class Parser {
    private:
        std::string filename;
    public:
        Parser(const std::string& filename);
        std::vector<T*> parse();
};

class ParserUtil {
    public:
        static bool isNumber(const std::string& string);
        static int getInt(std::istringstream& stream);
        static std::string getString(std::istringstream& stream);
        static std::string toString(const int& num);
        static std::string toString(const std::string& string);
};

#endif