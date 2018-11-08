// #include <vector>
// #include <cstring>
// #include <stdlib.h>

// #include "object.h"
// #include "dungeon.h"
// #include "utils.h"

// void generate_objects(dungeon_t *d)
// {
//     uint32_t i;
//     d->num_objects = 10;
//     object *o;
//     uint32_t room;
//     pair_t p;

//     std::vector<object_description> &ov = d->object_descriptions;
//     object_description &od = ov[rand_range(0, ov.size() - 1)];

//     for (i = 0; i < 10; i++)
//     {
//         o = od.gen_object();

//         room = rand_range(0, d->num_rooms - 1);
//         p[dim_y] = rand_range(d->rooms[room].position[dim_y],
//                               (d->rooms[room].position[dim_y] +
//                                d->rooms[room].size[dim_y] - 1));
//         p[dim_x] = rand_range(d->rooms[room].position[dim_x],
//                               (d->rooms[room].position[dim_x] +
//                                d->rooms[room].size[dim_x] - 1));

//         o->position[dim_y] = p[dim_y];
//         o->position[dim_x] = p[dim_x];
//         d->objmap[p[dim_y]][p[dim_x]] = o;
//     }
// }

// int16_t *object_get_pos(object *o)
// {
//     return o->position;
// }

// void destroy_objects(dungeon_t *d)
// {
//     uint32_t y, x;

//     for (y = 0; y < DUNGEON_Y; y++)
//     {
//         for (x = 0; x < DUNGEON_X; x++)
//         {
//             if (d->objmap[y][x])
//             {
//                 delete d->objmap[y][x];
//             }
//         }
//     }
// }
