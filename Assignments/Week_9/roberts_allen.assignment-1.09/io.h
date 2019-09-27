#ifndef IO_H
#define IO_H

#include "object.h"

class dungeon;

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display(dungeon *d);
void io_handle_input(dungeon *d);
void io_queue_message(const char *format, ...);

void display_inventory(dungeon *d);
void display_equipment(dungeon *d);
void inspect_item(dungeon *d);
void display_monster_desc(dungeon *d);
void select_monster(dungeon *d);

uint32_t destroy_equip(dungeon_t *d);
uint32_t look_at_inventory(dungeon_t *d);
void show_equip_inventory(dungeon_t *d);
uint32_t remove_equip(dungeon_t *d);
void open_inventory(dungeon_t *d);
uint32_t put_equip_on(dungeon_t *d);

void io_display_eq(dungeon_t *d);
uint32_t io_display_obj_info(object *o);
uint32_t io_drop_in(dungeon_t *d);
uint32_t io_inspect_eq(dungeon_t *d);
void io_object_to_string(object *o, char *s, uint32_t size);

#endif
