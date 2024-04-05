#include "Parsing.h"
#include <string>
#include <tuple>

#ifndef STAT_H
#define STAT_H

class Stat : public Data {
    private:
        std::tuple
        <
            int,
            int, 
            std::string,
            int,
            int
        > data;

    public:
        Stat(std::string line);

        auto getData() const { return data; }

        std::string toString() const;

        static std::vector<Data*> parse(const std::string& filename);
};

#endif