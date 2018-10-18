#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <ncurses.h>

/* Very slow seed: 686846853 */

#include "dungeon.h"

#include "pc.h"
#include "npc.h"
#include "move.h"

#define True 0
#define False 1

typedef struct monster
{
  char symbol;
  pair_t position;
} monster_t;

void printMonsters(dungeon_t *d, int startNum, monster_t monsters[], int count);
void showMonsters(dungeon_t *d, monster_t monsters[], int input, int count);

void clear_screen();
void menu(dungeon_t *d, int input);

const char *victory =
    "\n                                       o\n"
    "                                      $\"\"$o\n"
    "                                     $\"  $$\n"
    "                                      $$$$\n"
    "                                      o \"$o\n"
    "                                     o\"  \"$\n"
    "                oo\"$$$\"  oo$\"$ooo   o$    \"$    ooo\"$oo  $$$\"o\n"
    "   o o o o    oo\"  o\"      \"o    $$o$\"     o o$\"\"  o$      \"$  "
    "\"oo   o o o o\n"
    "   \"$o   \"\"$$$\"   $$         $      \"   o   \"\"    o\"         $"
    "   \"o$$\"    o$$\n"
    "     \"\"o       o  $          $\"       $$$$$       o          $  ooo"
    "     o\"\"\n"
    "        \"o   $$$$o $o       o$        $$$$$\"       $o        \" $$$$"
    "   o\"\n"
    "         \"\"o $$$$o  oo o  o$\"         $$$$$\"        \"o o o o\"  "
    "\"$$$  $\n"
    "           \"\" \"$\"     \"\"\"\"\"            \"\"$\"            \""
    "\"\"      \"\"\" \"\n"
    "            \"oooooooooooooooooooooooooooooooooooooooooooooooooooooo$\n"
    "             \"$$$$\"$$$$\" $$$$$$$\"$$$$$$ \" \"$$$$$\"$$$$$$\"  $$$\""
    "\"$$$$\n"
    "              $$$oo$$$$   $$$$$$o$$$$$$o\" $$$$$$$$$$$$$$ o$$$$o$$$\"\n"
    "              $\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""
    "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"$\n"
    "              $\"                                                 \"$\n"
    "              $\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\""
    "$\"$\"$\"$\"$\"$\"$\"$\n"
    "                                   You win!\n\n";

const char *tombstone =
    "\n\n\n\n                /\"\"\"\"\"/\"\"\"\"\"\"\".\n"
    "               /     /         \\             __\n"
    "              /     /           \\            ||\n"
    "             /____ /   Rest in   \\           ||\n"
    "            |     |    Pieces     |          ||\n"
    "            |     |               |          ||\n"
    "            |     |   A. Luser    |          ||\n"
    "            |     |               |          ||\n"
    "            |     |     * *   * * |         _||_\n"
    "            |     |     *\\/* *\\/* |        | TT |\n"
    "            |     |     *_\\_  /   ...\"\"\"\"\"\"| |"
    "| |.\"\"....\"\"\"\"\"\"\"\".\"\"\n"
    "            |     |         \\/..\"\"\"\"\"...\"\"\""
    "\\ || /.\"\"\".......\"\"\"\"...\n"
    "            |     |....\"\"\"\"\"\"\"........\"\"\"\"\""
    "\"^^^^\".......\"\"\"\"\"\"\"\"..\"\n"
    "            |......\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"......"
    "..\"\"\"\"\"....\"\"\"\"\"..\"\"...\"\"\".\n\n"
    "            You're dead.  Better luck in the next life.\n\n\n";

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "          [-s|--save [<file>]] [-i|--image <pgm file>]\n"
          "          [-n|--nummon <count>]",
          name);

  exit(-1);
}

int main(int argc, char *argv[])
{
  dungeon_t d;
  time_t seed;
  struct timeval tv;
  uint32_t i;
  uint32_t do_load, do_save, do_seed, do_image, do_save_seed, do_save_image;
  uint32_t long_arg;
  char *save_file;
  char *load_file;
  char *pgm_file;

  /* Quiet a false positive from valgrind. */
  memset(&d, 0, sizeof(d));

  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = do_save_seed = do_save_image = 0;
  do_seed = 1;
  save_file = load_file = NULL;
  d.max_monsters = MAX_MONSTERS;

  /* The project spec requires '--load' and '--save'.  It's common  *
   * to have short and long forms of most switches (assuming you    *
   * don't run out of letters).  For now, we've got plenty.  Long   *
   * forms use whole words and take two dashes.  Short forms use an *
    * abbreviation after a single dash.  We'll add '--rand' (to     *
   * specify a random seed), which will take an argument of it's    *
   * own, and we'll add short forms for all three commands, '-l',   *
   * '-s', and '-r', respectively.  We're also going to allow an    *
   * optional argument to load to allow us to load non-default save *
   * files.  No means to save to non-default locations, however.    *
   * And the final switch, '--image', allows me to create a dungeon *
   * from a PGM image, so that I was able to create those more      *
   * interesting test dungeons for you.                             */

  if (argc > 1)
  {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0)
    {
      if (argv[i][0] == '-')
      { /* All switches start with a dash */
        if (argv[i][1] == '-')
        {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1])
        {
        case 'n':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-nummon")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%hu", &d.max_monsters))
          {
            usage(argv[0]);
          }
          break;
        case 'r':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-rand")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */)
          {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        case 'l':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-load")))
          {
            usage(argv[0]);
          }
          do_load = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-')
          {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            load_file = argv[++i];
          }
          break;
        case 's':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-save")))
          {
            usage(argv[0]);
          }
          do_save = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-')
          {
            /* There is another argument, and it's not a switch, so *
             * we'll save to it.  If it is "seed", we'll save to    *
	     * <the current seed>.rlg327.  If it is "image", we'll  *
	     * save to <the current image>.rlg327.                  */
            if (!strcmp(argv[++i], "seed"))
            {
              do_save_seed = 1;
              do_save_image = 0;
            }
            else if (!strcmp(argv[i], "image"))
            {
              do_save_image = 1;
              do_save_seed = 0;
            }
            else
            {
              save_file = argv[i];
            }
          }
          break;
        case 'i':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-image")))
          {
            usage(argv[0]);
          }
          do_image = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-')
          {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            pgm_file = argv[++i];
          }
          break;
        default:
          usage(argv[0]);
        }
      }
      else
      { /* No dash */
        usage(argv[0]);
      }
    }
  }

  if (do_seed)
  {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  if (!do_load && !do_image)
  {
    printf("Seed is %ld.\n", seed);
  }
  srand(seed);

  init_dungeon(&d);

  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  refresh();

  if (do_load)
  {
    read_dungeon(&d, load_file);
  }
  else if (do_image)
  {
    read_pgm(&d, pgm_file);
  }
  else
  {
    gen_dungeon(&d);
  }

  /* Ignoring PC position in saved dungeons.  Not a bug. */
  config_pc(&d);
  gen_monsters(&d);
  char dir;

  uint32_t pause = False;

  do
  {
    if (pause == False)
    {
      render_dungeon(&d);
    }

    dir = getchar();

    if (pause == True)
    {
      if (dir == 27)
      {
        pause = False;
        clear_screen();
        render_dungeon(&d);
      }
      else
      {
        menu(&d, dir);
      }
      refresh();
    }
    else
    {
      if (dir == 'm')
      {
        // do_moves(&d, dir, pause);
        pause = True;
        clear_screen();
        menu(&d, dir);
        refresh();
      }
      do_moves(&d, dir, pause);
    }
    if (!(pc_is_alive(&d) && dungeon_has_npcs(&d) && dir != 'Q'))
      endwin();
  } while (pc_is_alive(&d) && dungeon_has_npcs(&d) && dir != 'Q');

  // while (pc_is_alive(&d) && dungeon_has_npcs(&d))
  // {
  //   render_dungeon(&d);
  //   do_moves(&d);
  //   usleep(330000);
  // }

  // render_dungeon(&d);

  if (do_save)
  {
    if (do_save_seed)
    {
      /* 10 bytes for number, please dot, extention and null terminator. */
      save_file = malloc(18);
      sprintf(save_file, "%ld.rlg327", seed);
    }
    if (do_save_image)
    {
      if (!pgm_file)
      {
        fprintf(stderr, "No image file was loaded.  Using default.\n");
        do_save_image = 0;
      }
      else
      {
        /* Extension of 3 characters longer than image extension + null. */
        save_file = malloc(strlen(pgm_file) + 4);
        strcpy(save_file, pgm_file);
        strcpy(strchr(save_file, '.') + 1, "rlg327");
      }
    }
    write_dungeon(&d, save_file);

    if (do_save_seed || do_save_image)
    {
      free(save_file);
    }
  }

  endwin();
  printf("%s", pc_is_alive(&d) ? victory : tombstone);
  printf("You defended your life in the face of %u deadly beasts.\n"
         "You avenged the cruel and untimely murders of %u "
         "peaceful dungeon residents.\n",
         d.pc.kills[kill_direct], d.pc.kills[kill_avenged]);

  pc_delete(d.pc.pc);

  delete_dungeon(&d);

  return 0;
}

void clear_screen()
{
  int i, j;
  for (i = 0; i < DUNGEON_Y; i++)
  {
    for (j = 0; j < DUNGEON_X; j++)
    {
      mvprintw(i, j, " ");
    }
  }
}

void menu(dungeon_t *d, int input)
{
  int i, j;
  mvprintw(0, 30, "Num monsters = %d", d->num_monsters);
  int count = 0;

  for (j = 0; j < DUNGEON_Y; j++)
  {
    for (i = 0; i < DUNGEON_X; i++)
    {
      if (d->character[j][i] && d->character[j][i]->symbol != '@')
      {
        count++;
      }
    }
  }

  monster_t monsters[count];
  count = 0;
  for (j = 0; j < DUNGEON_Y; j++)
  {
    for (i = 0; i < DUNGEON_X; i++)
    {
      if (d->character[j][i] && d->character[j][i]->symbol != '@')
      {
        monsters[count].symbol = d->character[j][i]->symbol;
        monsters[count].position[dim_x] = i;
        monsters[count].position[dim_y] = j;

        count++;
      }
    }
  }
  showMonsters(d, monsters, input, count);
  //printMonsters(d, 0, monsters);
}

void printMonsters(dungeon_t *d, int startNum, monster_t monsters[], int count)
{
  clear();
  refresh();
  move(0, 0);
  int line = 2;
  int endNum = MAX_MONSTERS < count - startNum
                   ? startNum + MAX_MONSTERS - 1
                   : count - 1;
  mvprintw(0, 30, "Monsters List");
  for (int i = startNum; i <= endNum; i++)
  {
    move(line, 0);
    int diffY = monsters[i].position[dim_y] - d->pc.position[dim_y];
    int diffX = monsters[i].position[dim_x] - d->pc.position[dim_x];
    mvprintw(line, 30, "%c, %d %s %d %s",
             monsters[i].symbol,
             abs(diffY),
             diffY >= 0 ? "north" : "south",
             abs(diffX),
             diffX >= 0 ? "east" : "west");
    line++;
    refresh();
  }
}

void showMonsters(dungeon_t *d, monster_t monsters[], int input, int count)
{
  int scrollIndex = 0;
  // int input = -1;
  printMonsters(d, scrollIndex * MAX_MONSTERS, monsters, count);
  while (input != 27)
  {
    input = getch();
    switch (input)
    {
    case KEY_UP:
      if (scrollIndex > 0)
      {
        scrollIndex--;
      }
      printMonsters(d, scrollIndex * MAX_MONSTERS, monsters, count);
      break;
    case KEY_DOWN:
      if ((scrollIndex + 1) * MAX_MONSTERS < d->num_monsters)
      {
        scrollIndex++;
      }
      printMonsters(d, scrollIndex * MAX_MONSTERS, monsters, count);
      break;
    }
  }
  // clear();
  // render_dungeon(d);
}
