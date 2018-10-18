#include "move.h"

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

#include "dungeon.h"
#include "heap.h"
#include "move.h"
#include "npc.h"
#include "pc.h"
#include "character.h"
#include "utils.h"
#include "path.h"
#include "event.h"

#define True 0
#define False 1

void do_combat(dungeon_t *d, character_t *atk, character_t *def)
{
  if (def->alive)
  {
    def->alive = 0;
    if (def != &d->pc)
    {
      d->num_monsters--;
    }
    atk->kills[kill_direct]++;
    atk->kills[kill_avenged] += (def->kills[kill_direct] +
                                 def->kills[kill_avenged]);
  }
}

void move_character(dungeon_t *d, character_t *c, pair_t next)
{
  if (charpair(next) &&
      ((next[dim_y] != c->position[dim_y]) ||
       (next[dim_x] != c->position[dim_x])))
  {
    do_combat(d, c, charpair(next));
  }
  else
  {
    /* No character in new position. */

    d->character[c->position[dim_y]][c->position[dim_x]] = NULL;
    c->position[dim_y] = next[dim_y];
    c->position[dim_x] = next[dim_x];
    d->character[c->position[dim_y]][c->position[dim_x]] = c;
  }
}

void do_moves(dungeon_t *d, uint32_t dir, uint32_t pause)
{
  pair_t next;
  character_t *c;
  event_t *e;

  /* Remove the PC when it is PC turn.  Replace on next call.  This allows *
   * use to completely uninit the heap when generating a new level without *
   * worrying about deleting the PC.                                       */
  if (pause == False)
  {
    if (pc_is_alive(d))
    {
      /* The PC always goes first one a tie, so we don't use new_event().  *
     * We generate one manually so that we can set the PC sequence       *
     * number to zero.                                                   */
      e = malloc(sizeof(*e));
      e->type = event_character_turn;
      /* Hack: New dungeons are marked.  Unmark and ensure PC goes at d->time, *
     * otherwise, monsters get a turn before the PC.                         */
      if (d->is_new)
      {
        d->is_new = 0;
        e->time = d->time;
      }
      else
      {
        e->time = d->time + (1000 / d->pc.speed);
      }
      e->sequence = 0;
      e->c = &d->pc;
      heap_insert(&d->events, e);
    }

    while (pc_is_alive(d) &&
           (e = heap_remove_min(&d->events)) &&
           ((e->type != event_character_turn) || (e->c != &d->pc)))
    {
      d->time = e->time;
      if (e->type == event_character_turn)
      {
        c = e->c;
      }
      if (!c->alive)
      {
        if (d->character[c->position[dim_y]][c->position[dim_x]] == c)
        {
          d->character[c->position[dim_y]][c->position[dim_x]] = NULL;
        }
        if (c != &d->pc)
        {
          event_delete(e);
        }
        continue;
      }

      npc_next_pos(d, c, next);
      move_character(d, c, next);

      heap_insert(&d->events, update_event(d, e, 1000 / c->speed));
    }

    if (pc_is_alive(d) && e->c == &d->pc)
    {
      c = e->c;
      d->time = e->time;
      /* Kind of kludgey, but because the PC is never in the queue when   *
     * we are outside of this function, the PC event has to get deleted *
     * and recreated every time we leave and re-enter this function.    */
      e->c = NULL;
      event_delete(e);
      // pc_next_pos(d, next);
      // next[dim_x] += c->position[dim_x];
      // next[dim_y] += c->position[dim_y];
      // if (mappair(next) <= ter_floor) {
      //   mappair(next) = ter_floor_hall;
      // }
      // move_character(d, c, next);
      move_pc(d, dir, pause);

      dijkstra(d);
      dijkstra_tunnel(d);
    }
  }
}

void dir_nearest_wall(dungeon_t *d, character_t *c, pair_t dir)
{
  dir[dim_x] = dir[dim_y] = 0;

  if (c->position[dim_x] != 1 && c->position[dim_x] != DUNGEON_X - 2)
  {
    dir[dim_x] = (c->position[dim_x] > DUNGEON_X - c->position[dim_x] ? 1 : -1);
  }
  if (c->position[dim_y] != 1 && c->position[dim_y] != DUNGEON_Y - 2)
  {
    dir[dim_y] = (c->position[dim_y] > DUNGEON_Y - c->position[dim_y] ? 1 : -1);
  }
}

uint32_t against_wall(dungeon_t *d, character_t *c)
{
  return ((mapxy(c->position[dim_x] - 1,
                 c->position[dim_y]) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x] + 1,
                 c->position[dim_y]) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x],
                 c->position[dim_y] - 1) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x],
                 c->position[dim_y] + 1) == ter_wall_immutable));
}

uint32_t in_corner(dungeon_t *d, character_t *c)
{
  uint32_t num_immutable;

  num_immutable = 0;

  num_immutable += (mapxy(c->position[dim_x] - 1,
                          c->position[dim_y]) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x] + 1,
                          c->position[dim_y]) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x],
                          c->position[dim_y] - 1) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x],
                          c->position[dim_y] + 1) == ter_wall_immutable);

  return num_immutable > 1;
}

uint32_t move_pc(dungeon_t *d, uint32_t dir, uint32_t pause)
{
  int stairs = 0;
  pair_t nextMove;
  nextMove[dim_x] = d->pc.position[dim_x];
  nextMove[dim_y] = d->pc.position[dim_y];

  if (pause == False)
  { // not pause

    switch (dir)
    {

    // Up
    case '8':
    case 'k':
      nextMove[dim_y]--;
      break;

    // Up - Right
    case '9':
    case 'u':
      nextMove[dim_x]++;
      nextMove[dim_y]--;
      break;

    // Up - Left
    case '7':
    case 'y':
      nextMove[dim_x]--;
      nextMove[dim_y]--;
      break;

    // Right
    case '6':
    case 'l':
      nextMove[dim_x]++;
      break;

    // Left
    case '4':
    case 'h':
      nextMove[dim_x]--;
      break;

    // Down
    case '2':
    case 'j':
      nextMove[dim_y]++;
      break;

    // Down - Right
    case '3':
    case 'n':
      nextMove[dim_x]++;
      nextMove[dim_y]++;
      break;

    // Down - Left
    case '1':
    case 'b':
      nextMove[dim_x]--;
      nextMove[dim_y]++;
      break;

    // Up Stairs
    case '<':
      if (mappair(d->pc.position) == ter_stairs_up)
      {
        stairs = 1;
        uint32_t seq = d->character_sequence_number;
        delete_dungeon(d);
        init_dungeon(d);
        gen_dungeon(d);
        d->character_sequence_number = seq;
        config_pc(d);
        gen_monsters(d);
      }
      stairs = 0;
      break;

    // Down Stairs
    case '>':
      if (mappair(d->pc.position) == ter_stairs_down)
      {
        stairs = 1;
        uint32_t seq = d->character_sequence_number;
        delete_dungeon(d);
        init_dungeon(d);
        gen_dungeon(d);
        d->character_sequence_number = seq;
        config_pc(d);
        gen_monsters(d);
      }
      stairs = 0;
      break;

    // Rest for a turn
    case '5':
    case ' ': // space, may need to fix
      break;

    default:
      break;
    }
  }

  if (stairs)
  {
    return 0;
  }

  if ((dir != '>') && (dir != '<') && (mappair(nextMove) >= ter_floor))
  {
    move_character(d, &d->pc, nextMove);
    return 0;
  }
  //Add code to move character position based on input.
  return 1;
}
