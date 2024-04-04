#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <vector>
#include <string>

class Move {
private:
    int id;
    std::string identifier;
    int generation_id;
    int type_id;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target_id;
    int damage_class_id;
    int effect_id;
    int effect_chance;
    int contest_type_id;
    int contest_effect_id;
    int super_contest_effect_id;

public:
    Move(std::vector<std::string> tokens)
        : id(std::stoi(tokens[0])), identifier(tokens[1]), generation_id(std::stoi(tokens[2])),
          type_id(std::stoi(tokens[3])), power(std::stoi(tokens[4])), pp(std::stoi(tokens[5])),
          accuracy(std::stoi(tokens[6])), priority(std::stoi(tokens[7])), target_id(std::stoi(tokens[8])),
          damage_class_id(std::stoi(tokens[9])), effect_id(std::stoi(tokens[10])), effect_chance(std::stoi(tokens[11])),
          contest_type_id(std::stoi(tokens[12])), contest_effect_id(std::stoi(tokens[13])),
          super_contest_effect_id(std::stoi(tokens[14])) {}

    int getId() const { return id; }
    const std::string& getIdentifier() const { return identifier; }
    int getGenerationId() const { return generation_id; }
    int getTypeId() const { return type_id; }
    int getPower() const { return power; }
    int getPp() const { return pp; }
    int getAccuracy() const { return accuracy; }
    int getPriority() const { return priority; }
    int getTargetId() const { return target_id; }
    int getDamageClassId() const { return damage_class_id; }
    int getEffectId() const { return effect_id; }
    int getEffectChance() const { return effect_chance; }
    int getContestTypeId() const { return contest_type_id; }
    int getContestEffectId() const { return contest_effect_id; }
    int getSuperContestEffectId() const { return super_contest_effect_id; }

    std::string toString() const;
};

#endif