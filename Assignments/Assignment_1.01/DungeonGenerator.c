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

#define True 0
#define False 1

struct Room{
    int xSize;
    int ySize;
    int position;
};

void CreateBoard();
int fetchRand();
struct Room createRoom();\
void makeRoom(struct Room r);


int main(int argc, char *argv[]){
    //int count = 0;
    //CreateBoard();

    srand(time(NULL));

    for(int i = 0; i < 5; i++){
        struct Room r = createRoom();

        if(r.xSize > 2 && r.ySize > 1){
            printf("X = %d \t Y = %d \t pos = %d\n", r.xSize, r.ySize, r.position);
            makeRoom(r);
            printf("\n");
        } else {
            i--;
        }

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
    struct Room r0 = createRoom();
    // struct Room r1 = createRoom();
    // struct Room r2 = createRoom();
    // struct Room r3 = createRoom();
    // struct Room r4 = createRoom();

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


struct Room createRoom(){
    struct Room r;
    do{
        r.position = fetchRand(80);
        r.xSize = fetchRand(10);
        r.ySize = fetchRand(10);
    } while (r.xSize < 3 && r.ySize < 2);

    return r;
}


int roomsNotTouching(){

    return True;
}


void makeRoom(struct Room r){
    for(int i = 0; i < r.ySize; i++){
        for(int j = 0; j < r.xSize; j++){
            printf("*");
        }
        printf("\n");
    }
}

