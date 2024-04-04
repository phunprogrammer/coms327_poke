#include <vector>
#include <string>

#ifndef PARSING_H
#define PARSING_H

class ParserUtil {
    public:
        static bool isNumber(const std::string& string);
        static int getInt(std::istringstream& stream);
        static std::string getString(std::istringstream& stream);
        static std::string toString(const int& num);
        static std::string toString(const std::string& string);

        template<typename T>
        static std::vector<T*> parseFile(std::string& name, std::string& file);
};

class Data {
    public:
        virtual std::string toString() const = 0;
};
#endif