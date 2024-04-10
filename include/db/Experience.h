#include "Parsing.h"
#include <string>
#include <tuple>

#ifndef EXPERIENCE_H
#define EXPERIENCE_H

class Experience : public Data {
    private:
        std::tuple
        <
            int,
            int,
            int
        > data;

    public:
        Experience(std::string line);

        auto getData() const { return data; }

        std::string toString() const;

        static std::vector<Experience*> parse(const std::string& filename);
};

#endif