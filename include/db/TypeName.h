#include "Parsing.h"
#include <string>
#include <tuple>

#ifndef TYPENAME_H
#define TYPENAME_H

class TypeName : public Data {
    private:
        std::tuple
        <
            int,
            int, 
            std::string 
        > data;

    public:
        TypeName(std::string line);

        auto getData() const { return data; }

        std::string toString() const;

        static std::vector<Data*> parse(const std::string& filename);
};

#endif