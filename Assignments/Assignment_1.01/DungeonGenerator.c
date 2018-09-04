#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*  TODO
    [X] 1. Create 80x21 boarder
    [ ] 2. Create 1 random box in the boarder with rock walls
    [ ] 3. add more boxes, dont check for collision yet
    [ ] 4. check for collision now and fix
    [ ] 5. store box data so we can create the corridors later
    [ ] 6. very basic/shitty corridors (dont check for collision)
    [ ] 7. Check for collision in corredors and fix any issues
    

*/

void CreateBoard();
int fetchRand();

int main(int argc, char *argv[]){
    //int count = 0;

    srand(time(NULL));

    for(int i = 0; i < 5; i++){
        printf("%d\n", fetchRand(80));
    }

    // Okay, We're creating 5 random numbers between 0 and 80, so that'll make
    // our base coordinates. SO, now generate random numbers between specifications
    // in the assignment and whatever we want to have be the max.
    // This will be our room size, so then we can create a room generator with
    // these specs and figure out how to use structs so we can store it?
    // 

}

int fetchRand(int N){
    return (rand() % N );
}

void CreateBoard(){
    for(int we = 0; we < 21; we++){
        for(int ns = 0; ns < 79; ns++){
            
            
            if(we == 0 || we == 20){
                printf("_");
            } 

            if((ns == 0 || ns == 78) && we != 0 && we != 20){
                printf("|");
            } 

            if( we != 0 && we != 20 && ns < 77) {
                printf("*");
            }


        
        }
        printf("\n");
    }
}

