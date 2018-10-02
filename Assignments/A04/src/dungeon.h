#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <errno.h>

/* Very slow seed: 686846853 */

#include "heap.h"

#define DUMP_HARDNESS_IMAGES 0

/* Returns true if random float in [0,1] is less than *
 * numerator/denominator.  Uses only integer math.    */
#define rand_under(numerator, denominator) \
	(rand() < ((RAND_MAX / denominator) * numerator))

/* Returns random integer in [min, max]. */
#define rand_range(min, max) ((rand() % (((max) + 1) - (min))) + (min))
#define UNUSED(f) ((void)f)

typedef struct corridor_path
{
	heap_node_t *hn;
	uint8_t pos[2];
	uint8_t from[2];
	int32_t cost;
} corridor_path_t;

typedef enum dim
{
	dim_x,
	dim_y,
	num_dims
} dim_t;

typedef int16_t pair_t[num_dims];

#define DUNGEON_X 80
#define DUNGEON_Y 21
#define MIN_ROOMS 5
#define MAX_ROOMS 9
#define ROOM_MIN_X 4
#define ROOM_MIN_Y 2
#define ROOM_MAX_X 14
#define ROOM_MAX_Y 8
#define SAVE_DIR ".rlg327"
#define DUNGEON_SAVE_FILE "dungeon"
#define DUNGEON_SAVE_SEMANTIC "RLG327-F2018"
#define DUNGEON_SAVE_VERSION 0U

#define mappair(pair) (d->map[pair[dim_y]][pair[dim_x]])
#define mapxy(x, y) (d->map[y][x])
#define hardnesspair(pair) (d->hardness[pair[dim_y]][pair[dim_x]])
#define hardnessxy(x, y) (d->hardness[y][x])

typedef enum __attribute__((__packed__)) terrain_type
{
	ter_debug,
	ter_wall,
	ter_wall_immutable,
	ter_floor,
	ter_floor_room,
	ter_floor_hall,
} terrain_type_t;

typedef struct room
{
	pair_t position;
	pair_t size;
} room_t;

typedef struct dungeon
{
	uint32_t num_rooms;
	room_t *rooms;
	terrain_type_t map[DUNGEON_Y][DUNGEON_X];
	/* Since hardness is usually not used, it would be expensive to pull it *
   * into cache every time we need a map cell, so we store it in a        *
   * parallel array, rather than using a structure to represent the       *
   * cells.  We may want a cell structure later, but from a performanace  *
   * perspective, it would be a bad idea to ever have the map be part of  *
   * that structure.  Pathfinding will rehuire efficient use of the map,  *
   * and pulling in unnecessary data with each map cell would add a lot   *
   * of overhead to the memory system.                                    */
	uint8_t hardness[DUNGEON_Y][DUNGEON_X];
	pair_t pc;
	uint32_t ntmap[DUNGEON_Y][DUNGEON_X];
	uint32_t tmap[DUNGEON_Y][DUNGEON_X];
} dungeon_t;

static uint32_t in_room(dungeon_t *d, int16_t y, int16_t x);

static uint32_t adjacent_to_room(dungeon_t *d, int16_t y, int16_t x);

static uint32_t is_open_space(dungeon_t *d, int16_t y, int16_t x);

static int32_t corridor_path_cmp(const void *key, const void *with);

static void dijkstra_corridor(dungeon_t *d, pair_t from, pair_t to);

/* This is a cut-and-paste of the above.  The code is modified to  *
 * calculate paths based on inverse hardnesses so that we get a    *
 * high probability of creating at least one cycle in the dungeon. */
static void dijkstra_corridor_inv(dungeon_t *d, pair_t from, pair_t to);

/* Chooses a random point inside each room and connects them with a *
 * corridor.  Random internal points prevent corridors from exiting *
 * rooms in predictable locations.                                  */
static int connect_two_rooms(dungeon_t *d, room_t *r1, room_t *r2);

static int create_cycle(dungeon_t *d);

static int connect_rooms(dungeon_t *d);

int gaussian[5][5] = {
	{1, 4, 7, 4, 1},
	{4, 16, 26, 16, 4},
	{7, 26, 41, 26, 7},
	{4, 16, 26, 16, 4},
	{1, 4, 7, 4, 1}};

typedef struct hueue_node
{
	int x, y;
	struct hueue_node *next;
} hueue_node_t;

static int smooth_hardness(dungeon_t *d);
static int empty_dungeon(dungeon_t *d);

static int place_rooms(dungeon_t *d);

static int make_rooms(dungeon_t *d);

int gen_dungeon(dungeon_t *d);

void render_dungeon(dungeon_t *d);

void delete_dungeon(dungeon_t *d);

void init_dungeon(dungeon_t *d);

int write_dungeon_map(dungeon_t *d, FILE *f);

int write_rooms(dungeon_t *d, FILE *f);

uint32_t calculate_dungeon_size(dungeon_t *d);

int makedirectory(char *dir);

int write_dungeon(dungeon_t *d, char *file);

int read_dungeon_map(dungeon_t *d, FILE *f);

int read_rooms(dungeon_t *d, FILE *f);
int calculate_num_rooms(uint32_t dungeon_bytes);

int read_dungeon(dungeon_t *d, char *file);

int read_pgm(dungeon_t *d, char *pgm);

void usage(char *name);

///////////////////////////////////////////////////////////////////////////////
/*                                My Code                                    */
///////////////////////////////////////////////////////////////////////////////


#define distancepair(pair) (d->hardness[pair[dim_y]][pair[dim_x]]/85 +1)

void my_Dijkstra_tm(dungeon_t *d);
void my_Dijkstra_ntm(dungeon_t *d);

void render_dijkstra_tm(dungeon_t *d);

void render_dijkstra_ntm(dungeon_t *d);

///////////////////////////////////////////////////////////////////////////////
/*                                My Code End                                */
///////////////////////////////////////////////////////////////////////////////

