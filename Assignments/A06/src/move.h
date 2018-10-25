#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>

#include "dims.h"

typedef struct dungeon dungeon_t;

class character;
//typedef struct character character;

void next_move(dungeon_t *d,
               character *c,

               pair_t goal_pos,
               pair_t next_pos);
void do_moves(dungeon_t *d);
void dir_nearest_wall(dungeon_t *d, character *c, pair_t dir);
uint32_t in_corner(dungeon_t *d, character *c);
uint32_t against_wall(dungeon_t *d, character *c);
uint32_t move_pc(dungeon_t *d, uint32_t dir);
void move_character(dungeon_t *d, character *c, pair_t next);
uint32_t teleport_pc(dungeon_t *d, uint32_t dir);

#endif
