#ifndef DB_PARSE_H
# define DB_PARSE_H

struct pokemon_db {
  int id;
  char identifier[30];
  int species_id;
  int height;
  int weight;
  int base_experience;
  int order;
  int is_default;
};

struct move_db {
  int id;
  char identifier[50];
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
};

struct pokemon_move_db {
  int pokemon_id;
  int version_group_id;
  int move_id;
  int pokemon_move_method_id;
  int level;
  int order;

  bool operator==(const pokemon_move_db& other) const {
    return (pokemon_id == other.pokemon_id) && (move_id == other.move_id);
  }
};

struct levelup_move {
  int level;
  int move;
};

struct pokemon_species_db {
  int id;
  char identifier[30];
  int generation_id;
  int evolves_from_species_id;
  int evolution_chain_id;
  int color_id;
  int shape_id;
  int habitat_id;
  int gender_rate;
  int capture_rate;
  int base_happiness;
  int is_baby;
  int hatch_counter;
  int has_gender_differences;
  int growth_rate_id;
  int forms_switchable;
  int is_legendary;
  int is_mythical;
  int order;
  int conquest_order;
};

struct experience_db {
  int growth_rate_id;
  int level;
  int experience;
};

struct pokemon_stats_db {
  int pokemon_id;
  int stat_id;
  int base_stat;
  int effort;
};

struct stats_db {
  int id;
  int damage_class_id;
  char identifier[30];
  int is_battle_only;
  int game_index;
};

struct pokemon_types_db {
  int pokemon_id;
  int type_id;
  int slot;
};

extern const int pokemon_db_size;
extern pokemon_db pokemon[];

extern const int pokemon_moves_size;
extern pokemon_move_db pokemon_moves[];

extern const int types_size;
extern char *types[];

extern const int moves_size;
extern move_db moves[];

extern const int pokemon_species_size;
extern pokemon_species_db pokemon_species[];

extern const int experience_size;
extern experience_db experience[];

extern const int pokemon_stats_size;
extern pokemon_stats_db pokemon_stats[];

extern const int stats_size;
extern stats_db stats[];

extern const int pokemon_types_size;
extern pokemon_types_db pokemon_types[];

void db_parse(bool print);

#endif
