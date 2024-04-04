#include "Pokemon.h"

std::string Pokemon::toString() const {
    return std::to_string(id) + "," + identifier + "," + std::to_string(species_id) + "," +
            std::to_string(height) + "," + std::to_string(weight) + "," +
            std::to_string(base_experience) + "," + std::to_string(order) + "," +
            std::to_string(is_default);
}