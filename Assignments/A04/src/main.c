#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/* Very slow seed: 686846853 */

#include "dungeon.h"
#include "path.h"
#include "utils.h"

#define TRUE  1
#define FALSE 0

enum test{
  up,
  down,
  left,
  right,
  up_left,
  up_right,
  down_left,
  down_right
};

int canMove(dungeon_t *d, int x, int y);
void temp(dungeon_t *d, ntmob_t *ntmob, int i);
int is_t_mob(dungeon_t *d, int x, int y);
int is_nt_mob(dungeon_t *d, int x, int y);

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "          [-s|--save [<file>]] [-i|--image <pgm file>]\n"
          "          [-n|--nummon <num_of_monster>]\n",
          name);

  exit(-1);
}

///////////////////////////////////////////////////////////////////////////////
/*                                My Code                                    */
///////////////////////////////////////////////////////////////////////////////

void move_pc(dungeon_t *d){
  int r = rand_range(0,6);

  switch(r){
    case up: // [ 0, -1]
      if(d->hardness[d->pc.position[dim_y] - 1][d->pc.position[dim_x]] == 0){
        d->pc.position[dim_x] = d->pc.position[dim_x];
        d->pc.position[dim_y] = d->pc.position[dim_y] - 1; 
      }else {
        move_pc(d);
      }
      break;

    case down: // [ 0, 1]
      if(d->hardness[d->pc.position[dim_y] + 1][d->pc.position[dim_x]] == 0){
        d->pc.position[dim_x] = d->pc.position[dim_x];
        d->pc.position[dim_y] = d->pc.position[dim_y]  + 1;
      } else {
        move_pc(d);
      }
      break;
      
    case left: // [ -1, 0]
      if(d->hardness[d->pc.position[dim_y]][d->pc.position[dim_x] - 1] == 0){
        d->pc.position[dim_x] = d->pc.position[dim_x] - 1;
        d->pc.position[dim_y] = d->pc.position[dim_y];
      } else {
        move_pc(d);
      }
      break;

    case right: // [ 1, 0]
      if(d->hardness[d->pc.position[dim_y]][d->pc.position[dim_x] + 1] == 0){
        d->pc.position[dim_x] = d->pc.position[dim_x] + 1;
        d->pc.position[dim_y] = d->pc.position[dim_y];
      } else {
        move_pc(d);
      }
      break;

    case up_left: // [ -1, -1]
      if(d->hardness[d->pc.position[dim_y] - 1][d->pc.position[dim_x] - 1] == 0){
        d->pc.position[dim_x] = d->pc.position[dim_x] - 1;
        d->pc.position[dim_y] = d->pc.position[dim_y] - 1;
      } else {
        move_pc(d);
      }
      break;

    case up_right: // [ 1, -1]
      if(d->hardness[d->pc.position[dim_y] - 1][d->pc.position[dim_x] + 1] == 0){
        d->pc.position[dim_x] = d->pc.position[dim_x] + 1;
        d->pc.position[dim_y] = d->pc.position[dim_y] - 1;
      } else {
        move_pc(d);
      }
      break;

    case down_left: // [ -1, 1]
      if(d->hardness[d->pc.position[dim_y] + 1][d->pc.position[dim_x] - 1] == 0){
        d->pc.position[dim_x] = d->pc.position[dim_x] - 1;
        d->pc.position[dim_y] = d->pc.position[dim_y] + 1;
      } else {
        move_pc(d);
      }
      break;

    case down_right: // [ 1, 1]
      if(d->hardness[d->pc.position[dim_y] + 1][d->pc.position[dim_x] + 1] == 0){
        d->pc.position[dim_x] = d->pc.position[dim_x] + 1;
        d->pc.position[dim_y] = d->pc.position[dim_y] + 1;
      } else {
        move_pc(d);
      }
      break;

    default:
      break;
  }
}

void move_ntmob(dungeon_t *d,ntmob_t *ntmob){
  int r = rand_range(0,6);
    switch(r){
      case up: // [ 0, -1]
        if(d->hardness[ntmob->position[dim_y] - 1][ntmob->position[dim_x]] == 0){
          ntmob->position[dim_x] = ntmob->position[dim_x];
          ntmob->position[dim_y] = ntmob->position[dim_y] - 1; 
        }
        break;

      case down: // [ 0, 1]
        if(d->hardness[ntmob->position[dim_y] + 1][ntmob->position[dim_x]] == 0){
          ntmob->position[dim_x] = ntmob->position[dim_x];
          ntmob->position[dim_y] = ntmob->position[dim_y]  + 1;
        }
        break; 
        
      case left: // [ -1, 0]
        if(d->hardness[ntmob->position[dim_y]][ntmob->position[dim_x] - 1] == 0){
          ntmob->position[dim_x] = ntmob->position[dim_x] - 1;
          ntmob->position[dim_y] = ntmob->position[dim_y];
        }
        break;

      case right: // [ 1, 0]
        if(d->hardness[ntmob->position[dim_y]][ntmob->position[dim_x] + 1] == 0){
          ntmob->position[dim_x] = ntmob->position[dim_x] + 1;
          ntmob->position[dim_y] = ntmob->position[dim_y];
        }
        break;

      case up_left: // [ -1, -1]
        if(d->hardness[ntmob->position[dim_y] - 1][ntmob->position[dim_x] - 1] == 0){
          ntmob->position[dim_x] = ntmob->position[dim_x] - 1;
          ntmob->position[dim_y] = ntmob->position[dim_y] - 1;
        }
        break;

      case up_right: // [ 1, -1]
        if(d->hardness[ntmob->position[dim_y] - 1][ntmob->position[dim_x] + 1] == 0){
          ntmob->position[dim_x] = ntmob->position[dim_x] + 1;
          ntmob->position[dim_y] = ntmob->position[dim_y] - 1;
        }
        break; 

      case down_left: // [ -1, 1]
        if(d->hardness[ntmob->position[dim_y] + 1][ntmob->position[dim_x] - 1] == 0){
          ntmob->position[dim_x] = ntmob->position[dim_x] - 1;
          ntmob->position[dim_y] = ntmob->position[dim_y] + 1;
        }
        break;

      case down_right: // [ 1, 1]
        if(d->hardness[ntmob->position[dim_y] + 1][ntmob->position[dim_x] + 1] == 0){
          ntmob->position[dim_x] = ntmob->position[dim_x] + 1;
          ntmob->position[dim_y] = ntmob->position[dim_y] + 1;
        }
        break;

      default:
        move_ntmob(d, ntmob);
        break;
    }

    if(ntmob->position[dim_x] == d->pc.position[dim_x] && 
      ntmob->position[dim_y] == d->pc.position[dim_y] ){
        d->pc.alive = FALSE;
    }
    int i;
    int count;
    count = 0;
    for(i = 0; i < d->num_nt_mobs; i++){
      if (d->ntmob[i].position[dim_x] == ntmob->position[dim_x] && d->ntmob[i].position[dim_y] == ntmob->position[dim_y] && d->ntmob[i].alive == TRUE){
        count++;
      }
    }
    if(count > 1){
      ntmob->alive = FALSE;
    }

      if(is_t_mob(d, ntmob->position[dim_x], ntmob->position[dim_y])){
        // is = is_t_mob(d, ntmob->position[dim_x], ntmob->position[dim_y]);
        // d->tmob[is].alive = FALSE;
        ntmob->alive = FALSE;
      }
}

void move_tmob(dungeon_t *d,tmob_t *tmob){
  int r = rand_range(0,6);
    switch(r){
      case up: // [ 0, -1]
        // if(d->hardness[tmob->position[dim_y] - 1][tmob->position[dim_x]] < 255){
        //   if(d->hardness[tmob->position[dim_y] - 1][tmob->position[dim_x]] <= 85){
        //     d->hardness[tmob->position[dim_y] - 1][tmob->position[dim_x]] = 0;
        //     tmob->position[dim_x] = tmob->position[dim_x];
        //     tmob->position[dim_y] = tmob->position[dim_y] - 1;
        //   } else{
        //     d->hardness[tmob->position[dim_y -1]][tmob->position[dim_x]] -= 85;
        //   }
        // }
        break;

      case down: // [ 0, 1]
        if(d->hardness[tmob->position[dim_y] + 1][tmob->position[dim_x]] < 255){
          if(d->hardness[tmob->position[dim_y] + 1][tmob->position[dim_x]] <= 85){
            d->hardness[tmob->position[dim_y] + 1][tmob->position[dim_x]] = 0;
            tmob->position[dim_x] = tmob->position[dim_x];
            tmob->position[dim_y] = tmob->position[dim_y] + 1;
          } else{
            d->hardness[tmob->position[dim_y+1]][tmob->position[dim_x]] -= 85;
          }
        }
        break; 
        
      case left: // [ -1, 0]
        if(d->hardness[tmob->position[dim_y]][tmob->position[dim_x] - 1] < 255){
        if(d->hardness[tmob->position[dim_y]][tmob->position[dim_x] - 1] <= 85){
            d->hardness[tmob->position[dim_y]][tmob->position[dim_x] - 1] = 0;
            tmob->position[dim_x] = tmob->position[dim_x] - 1;
            tmob->position[dim_y] = tmob->position[dim_y];
          } else{
            d->hardness[tmob->position[dim_y]][tmob->position[dim_x] -1] -= 85;
          }
        }
        break;

      case right: // [ 1, 0]
        if(d->hardness[tmob->position[dim_y]][tmob->position[dim_x] + 1] < 255){
          if(d->hardness[tmob->position[dim_y]][tmob->position[dim_x] + 1] <= 85){
            d->hardness[tmob->position[dim_y]][tmob->position[dim_x] + 1] = 0;
            tmob->position[dim_x] = tmob->position[dim_x] + 1;
            tmob->position[dim_y] = tmob->position[dim_y];
          } else{
            d->hardness[tmob->position[dim_y]][tmob->position[dim_x] +1] -= 85;
          }
        }
        break;

      case up_left: // [ -1, -1]
        // if(d->hardness[tmob->position[dim_y] - 1][tmob->position[dim_x] - 1] < 255){
        //   if(d->hardness[tmob->position[dim_y] - 1][tmob->position[dim_x] - 1] <= 85){
        //       d->hardness[tmob->position[dim_y] - 1][tmob->position[dim_x] - 1] = 0;
        //       tmob->position[dim_x] = tmob->position[dim_x] - 1;
        //       tmob->position[dim_y] = tmob->position[dim_y] - 1;
        //     } else{
        //       d->hardness[tmob->position[dim_y]-1][tmob->position[dim_x]-1] -= 85;
        //     }
        // }
        break;

      case up_right: // [ 1, -1]
        // if(d->hardness[tmob->position[dim_y] - 1][tmob->position[dim_x] + 1] < 255 ){
        //   if(d->hardness[tmob->position[dim_y] - 1][tmob->position[dim_x] + 1] <= 85){
        //     d->hardness[tmob->position[dim_y] - 1][tmob->position[dim_x] + 1] = 0;
        //     tmob->position[dim_x] = tmob->position[dim_x] + 1;
        //     tmob->position[dim_y] = tmob->position[dim_y] - 1;
        //   } else{
        //     d->hardness[tmob->position[dim_y]-1][tmob->position[dim_x]+1] -= 85;
        //   }
        // }
        break;

      case down_left: // [ -1, 1]
        if(d->hardness[tmob->position[dim_y] + 1][tmob->position[dim_x] - 1] < 255){
          if(d->hardness[tmob->position[dim_y] + 1][tmob->position[dim_x] - 1] <= 85){
            d->hardness[tmob->position[dim_y] + 1][tmob->position[dim_x] - 1] = 0;
            tmob->position[dim_x] = tmob->position[dim_x] - 1;
            tmob->position[dim_y] = tmob->position[dim_y] + 1;
          } else{
            d->hardness[tmob->position[dim_y]+1][tmob->position[dim_x]-1] -= 85;
          }
        }
        break;

      case down_right: // [ 1, 1]
        if(d->hardness[tmob->position[dim_y] + 1][tmob->position[dim_x] + 1] < 255){
          if(d->hardness[tmob->position[dim_y] + 1][tmob->position[dim_x]+ 1] <= 85){
            d->hardness[tmob->position[dim_y] + 1][tmob->position[dim_x] + 1] = 0;
            tmob->position[dim_x] = tmob->position[dim_x] + 1;
            tmob->position[dim_y] = tmob->position[dim_y] + 1;
          } else{
            d->hardness[tmob->position[dim_y]+1][tmob->position[dim_x]+1] -= 85;
          }
        }
        break;

      default:
        move_tmob(d, tmob);
        break;
    }
    //int is;
    if(tmob->position[dim_x] == d->pc.position[dim_x] && 
      tmob->position[dim_y] == d->pc.position[dim_y] ){
        d->pc.alive = FALSE;
    }
    int i;
    int count;
    count = 0;

    for(i = 0; i < d->num_nt_mobs; i++){
      if (d->tmob[i].position[dim_x] == tmob->position[dim_x] && d->tmob[i].position[dim_y] == tmob->position[dim_y] && d->tmob[i].alive == TRUE){
        count++;
      }
    }
    if(count > 1){
      tmob->alive = FALSE;
    }

    if(is_t_mob(d, tmob->position[dim_x], tmob->position[dim_y])){
      tmob->alive = FALSE;
    }
}

void place_mobs(dungeon_t *d){
  // Place Non-Tunnelling mobs
  int i;
  for(i = 0; i < d->num_nt_mobs; i++){
    if(i < d->num_rooms){
      d->ntmob[i].position[dim_x] = (d->rooms[i].position[dim_x] + (rand() % d->rooms[i].size[dim_x]));
      d->ntmob[i].position[dim_y] = (d->rooms[i].position[dim_y] + (rand() % d->rooms[i].size[dim_y]));
    }
  }

  // Place Tunnelling mobs
  for(i = 0; i < d->num_t_mobs; i++){
    d->tmob[i].position[dim_x] = i + 1;
    d->tmob[i].position[dim_y] = 1;
    d->hardness[d->tmob[i].position[dim_y]][d->tmob[i].position[dim_x]] = 0;
  }
}

int is_t_mob(dungeon_t *d, int x, int y){
  int i;
  for(i = 0; i < d->num_t_mobs; i++){
    if (d->tmob[i].position[dim_x] == x && d->tmob[i].position[dim_y] == y){
      return i * -1;
    }
  }
  return TRUE;
}

int is_nt_mob(dungeon_t *d, int x, int y){
  int i;
  for(i = 0; i < d->num_nt_mobs; i++){
    if (d->ntmob[i].position[dim_x] == x && d->ntmob[i].position[dim_y] == y){
      return FALSE;
    }
  }
  return TRUE;
}

void create_mobs(dungeon_t *d, int num){
  int i;
  int size;
  if(num <= d->num_rooms){
    size = rand_range(0,num);
  } else {
    size = d->num_rooms;
  }

  d->num_nt_mobs = size;
  d->num_t_mobs = num - d->num_nt_mobs;

  d->ntmob = malloc(sizeof(d->ntmob) * d->num_nt_mobs);
  d->tmob = malloc(sizeof(d->tmob) * d->num_t_mobs);
  
  printf("Num NT: %d\t T: %d\n", d->num_nt_mobs, d->num_t_mobs);
  for(i = 0; i < d->num_nt_mobs; i++){
    d->ntmob[i].position[dim_x] = 0;
    d->ntmob[i].position[dim_y] = 0;
    d->ntmob[i].alive = TRUE;
  }

  for(i = 0; i < d->num_t_mobs; i++){
    d->tmob[i].position[dim_x] = i + 2;
    d->tmob[i].position[dim_y] = 1;
    d->tmob[i].alive = TRUE;
  }

  place_mobs(d);
}

void print_game_over(){
  printf("\n\n\n\n\n \
                     ██▀▀▀▀██ ███▀▀▀███ ███▀█▄█▀███ ██▀▀▀ \n \
                     ██    ██ ██     ██ ██   █   ██ ██    \n \
                     ██   ▄▄▄ ██▄▄▄▄▄██ ██   ▀   ██ ██▀▀▀ \n \
                     ██    ██ ██     ██ ██       ██ ██    \n \
                     ██▄▄▄▄██ ██     ██ ██       ██ ██▄▄▄ \n \
                                                          \n \
                     ██▀▀▀▀▀██ ▀███  ██▀ ██▀▀▀ ██▀▀▀▀▀█▄  \n \
                     ██     ██   ██  ██  ██    ██     ██  \n \
                     ██     ██   ██  ██  ██▀▀▀ ██▄▄▄▄▄▀▀  \n \
                     ██     ██   ██  █▀  ██    ██    ██   \n \
                     ██▄▄▄▄▄██    ▀█▀    ██▄▄▄ ██     ██▄ \n \
  \n\n\n\n\n ");
}

///////////////////////////////////////////////////////////////////////////////
/*                                My Code End                                */
///////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
  dungeon_t d;
  time_t seed;
  struct timeval tv;
  uint32_t i;
  uint32_t do_load, do_save, do_seed, do_image, do_save_seed, do_save_image, do_nummon, nummons;
  uint32_t long_arg;
  char *save_file;
  char *load_file;
  char *pgm_file;

  /* Quiet a false positive from valgrind. */
  memset(&d, 0, sizeof (d));
  
  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = do_save_seed = do_save_image = do_nummon = 0;
  do_seed = 1;
  save_file = load_file = NULL;

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

 if (argc > 1) {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
      if (argv[i][0] == '-') { /* All switches start with a dash */
        if (argv[i][1] == '-') {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1]) {
        case 'n':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-nummon")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
            usage(argv[0]);
          }
          nummons = atoi(argv[i++]);
          if(nummons > 30){
            printf("Integer to large, MAX is 30. Using 30\n");
            nummons = 30;
          }
          do_nummon = 1;
          break;
        case 'r':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-rand")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        case 'l':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-load"))) {
            usage(argv[0]);
          }
          do_load = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            load_file = argv[++i];
          }
          break;
        case 's':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-save"))) {
            usage(argv[0]);
          }
          do_save = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll save to it.  If it is "seed", we'll save to    *
	     * <the current seed>.rlg327.  If it is "image", we'll  *
	     * save to <the current image>.rlg327.                  */
	    if (!strcmp(argv[++i], "seed")) {
	      do_save_seed = 1;
	      do_save_image = 0;
	    } else if (!strcmp(argv[i], "image")) {
	      do_save_image = 1;
	      do_save_seed = 0;
	    } else {
	      save_file = argv[i];
	    }
          }
          break;
        case 'i':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-image"))) {
            usage(argv[0]);
          }
          do_image = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            pgm_file = argv[++i];
          }
          break;
        default:
          usage(argv[0]);
        }
      } else { /* No dash */
        usage(argv[0]);
      }
    }
 }


  if (do_seed) {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  printf("Seed is %ld.\n", seed);
  srand(seed);

  init_dungeon(&d);

  if (do_load) {
    read_dungeon(&d, load_file);
   } else if (do_image) {
    read_pgm(&d, pgm_file);
   } else {
    gen_dungeon(&d);
  }

  if (!do_load) {
    /* Set a valid position for the PC */
    d.pc.position[dim_x] = (d.rooms[0].position[dim_x] +
                            (rand() % d.rooms[0].size[dim_x]));
    d.pc.position[dim_y] = (d.rooms[0].position[dim_y] +
                            (rand() % d.rooms[0].size[dim_y]));         
  }

  if(do_nummon){
    printf("Nummons: %d\n", nummons);
    create_mobs(&d, nummons);
  }
  if(!do_nummon){
    create_mobs(&d, d.num_rooms);
  }

  printf("PC is at (y, x): %d, %d\n",
         d.pc.position[dim_y], d.pc.position[dim_x]);

  d.pc.alive = TRUE;

  int counter = 0;

  while(d.pc.alive){
    usleep(300000);
    move_pc(&d);
    render_dungeon(&d);
    dijkstra(&d);
    dijkstra_tunnel(&d);
    int i;
    for(i = 0; i < d.num_nt_mobs; i++){
      //if(d.ntmob[i].alive == TRUE){
        move_ntmob(&d, &d.ntmob[i]);
      //}
    }
   //if( counter % 3 == 0){
      for(i = 0; i < d.num_t_mobs; i++){
        //if(d.tmob[i].alive == TRUE){
          move_tmob(&d, &d.tmob[i]);
        //}
        //move_tmob(&d, &d.tmob[i]);
      }
   // }
    counter++;
  }
  //render_dungeon(&d);

  print_game_over();

  
 //render_distance_map(&d);
//  render_tunnel_distance_map(&d);
//  render_hardness_map(&d);
//  render_movement_cost_map(&d);


  if (do_save) {
    if (do_save_seed) {
       /* 10 bytes for number, plus dot, extention and null terminator. */
      save_file = malloc(18);
      sprintf(save_file, "%ld.rlg327", seed);
    }
    if (do_save_image) {
      if (!pgm_file) {
        fprintf(stderr, "No image file was loaded.  Using default.\n");
        do_save_image = 0;
      } else {
        /* Extension of 3 characters longer than image extension + null. */
        save_file = malloc(strlen(pgm_file) + 4);
        strcpy(save_file, pgm_file);
        strcpy(strchr(save_file, '.') + 1, "rlg327");
      }
    }
    write_dungeon(&d, save_file);

    if (do_save_seed || do_save_image) {
      free(save_file);
    }
  }


  delete_dungeon(&d);

  return 0;
}
