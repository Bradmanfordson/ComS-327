#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "dice.h"
#include "character.h"
#include "descriptions.h"

class object
{
  public:
    std::string name;
    std::string description;
    char symbol;
    object_type_t type;
    uint32_t color;
    pair_t position;
    dice damage;
    int32_t weight,
        speed,
        attribute,
        value,
        hit,
        dodge,
        defense;
    bool seen;
};

void generate_objects(dungeon_t *d);
void destroy_objects(dungeon_t *d);

#endif