#include "Move.h"
#include "Config.h"
#include <string>

std::string Move::toString() const {
    return (id == MAX ? "" : std::to_string(id)) + "," +
           (identifier == std::to_string(MAX) ? "" : identifier) + "," +
           (generation_id == MAX ? "" : std::to_string(generation_id)) + "," +
           (type_id == MAX ? "" : std::to_string(type_id)) + "," +
           (power == MAX ? "" : std::to_string(power)) + "," +
           (pp == MAX ? "" : std::to_string(pp)) + "," +
           (accuracy == MAX ? "" : std::to_string(accuracy)) + "," +
           (priority == MAX ? "" : std::to_string(priority)) + "," +
           (target_id == MAX ? "" : std::to_string(target_id)) + "," +
           (damage_class_id == MAX ? "" : std::to_string(damage_class_id)) + "," +
           (effect_id == MAX ? "" : std::to_string(effect_id)) + "," +
           (effect_chance == MAX ? "" : std::to_string(effect_chance)) + "," +
           (contest_type_id == MAX ? "" : std::to_string(contest_type_id)) + "," +
           (contest_effect_id == MAX ? "" : std::to_string(contest_effect_id)) + "," +
           (super_contest_effect_id == MAX ? "" : std::to_string(super_contest_effect_id));
}