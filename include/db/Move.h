#include "Parsing.h"
#include <string>
#include <tuple>

#ifndef MOVE_H
#define MOVE_H

class Move : public Data {
    private:
        std::tuple
        <
            int, /*id*/
            std::string, /*identifier*/
            int, /*generation_id*/
            int, /*type_id*/
            int, /*power*/
            int, /*pp*/
            int, /*accuracy*/
            int, /*priority*/
            int, /*target_id*/
            int, /*damage_class_id*/
            int, /*effect_id*/
            int, /*effect_chance*/
            int, /*contest_type_id*/
            int, /*contest_effect_id*/
            int /*super_contest_effect_id*/
        > data;

    public:
        Move(std::string line);

        auto getData() const { return data; }

        std::string toString() const;

        static std::vector<Data*> parse(const std::string& filename);
};

#endif