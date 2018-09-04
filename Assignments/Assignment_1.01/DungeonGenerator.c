#include <stdio.h>
#include <stdlib.h>
#include "DungeonGenerator.h"

/*  TODO
    1. Create 80x21 boarder
    2. Create 1 random box in the boarder with rock walls
    3. add more boxes, dont check for collision yet
    4. check for collision now and fix
    5. store box data so we can create the corridors later
    6. very basic/shitty corridors (dont check for collision)
    7. Check for collision in corredors and fix any issues
    8. drink.

*/

void CreateBoard();

int main(int argc, char *argv[]){
    CreateBoard();


}

void CreateBoard(){
    for(int we=0; we < 21; we++){
        for(int ns = 0; ns < 80; ns++){
            if(we == 0 || we == 20){
                printf("-");
            }
            if(ns == 0 || ns == 79){
                printf("|");
            }
            if(ns == 80 || we == 20){
                printf("\n");
            }
        }
    }
}