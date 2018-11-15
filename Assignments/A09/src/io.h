#ifndef IO_H
#define IO_H

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

#endif
