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
    int xPos;
    int yPos;
    int xItter;
    int yItter;
};


void CreateBoard();
int fetchRand();
void createRooms();
void makeRoom(struct Room r);
void printRoomStats();
void checkCollision();

struct Room rooms[5];

int main(int argc, char *argv[]){
    //int count = 0;
    srand(time(NULL));
    createRooms();
    //checkCollision();
    CreateBoard();

    // for(int i = 0; i < 5; i++){
    //     struct Room r = createRoom();

    //     if(r.xSize > 2 && r.ySize > 1){
    //         printf("X = %d \t Y = %d \t xpos = %d \t ypos = %d\n", r.xSize, r.ySize, r.xPos, r.yPos);
    //         makeRoom(r);
    //         printf("\n");
    //     } else {
    //         i--;
    //     }

    // }

    // Okay, We're creating 5 random numbers between 0 and 80, so that'll make
    // our base coordinates. SO, now generate random numbers between specifications
    // in the assignment and whatever we want to have be the max.
    // This will be our room size, so then we can create a room generator with
    // these specs and figure out how to use structs so we can store it?
    // 

}


void CreateBoard(){

    for(int i = 0; i < 5; i++){
       printRoomStats(rooms[i]); 
    }
    
    for(int we = 0; we < 21; we++){
        for(int ns = 0; ns < 79; ns++){
            
            if(we == 0 || we == 20){
                printf("_");
            } 

            if((ns == 0 || ns == 78) && we != 0 && we != 20){
                printf("|");
            } 

            // if(we == (r0.yPos - 1) && ns == (r0.xPos - 2)){
            //     printf("~");

            // } else 
           
            if(we == (rooms[0].yPos - 1) && ns == (rooms[0].xPos - 2)){
                printf("~");
                if(rooms[0].xPos < rooms[0].xItter - 1 && 
                   rooms[0].yPos < rooms[0].yItter){
                    rooms[0].xPos++;
                } else {
                    rooms[0].xPos = rooms[0].xItter - rooms[0].xSize;
                    if(rooms[0].yPos < rooms[0].yItter - 1){
                        rooms[0].yPos++;
                    }
                    
                }
            } else if(we == (rooms[1].yPos - 1) && ns == (rooms[1].xPos - 2)){
                printf("~");
                if(rooms[1].xPos < rooms[1].xItter - 1 && 
                   rooms[1].yPos < rooms[1].yItter){
                    rooms[1].xPos++;
                } else {
                    rooms[1].xPos = rooms[1].xItter - rooms[1].xSize;
                    if(rooms[1].yPos < rooms[1].yItter - 1){
                        rooms[1].yPos++;
                    }
                    
                }
            } else if(we == (rooms[2].yPos - 1) && ns == (rooms[2].xPos - 2)){
                printf("~");
                if(rooms[2].xPos < rooms[2].xItter - 1 && 
                   rooms[2].yPos < rooms[2].yItter){
                    rooms[2].xPos++;
                } else {
                    rooms[2].xPos = rooms[2].xItter - rooms[2].xSize;
                    if(rooms[2].yPos < rooms[2].yItter - 1){
                        rooms[2].yPos++;
                    }
                    
                }
            } else if(we == (rooms[3].yPos - 1) && ns == (rooms[3].xPos - 2)){
                printf("~");
                if(rooms[3].xPos < rooms[3].xItter - 1 && 
                   rooms[3].yPos < rooms[3].yItter){
                    rooms[3].xPos++;
                } else {
                    rooms[3].xPos = rooms[3].xItter - rooms[3].xSize;
                    if(rooms[3].yPos < rooms[3].yItter - 1){
                        rooms[3].yPos++;
                    }
                    
                }
            } else if(we == (rooms[4].yPos - 1) && ns == (rooms[4].xPos - 2)){
                printf("~");
                if(rooms[4].xPos < rooms[4].xItter - 1 && 
                   rooms[4].yPos < rooms[4].yItter){
                    rooms[4].xPos++;
                } else {
                    rooms[4].xPos = rooms[4].xItter - rooms[4].xSize;
                    if(rooms[4].yPos < rooms[4].yItter - 1){
                        rooms[4].yPos++;
                    }
                    
                }
            } else if( we != 0 && we != 20 && ns < 77) {
                printf("@");
            }



        
        }
        printf("\n");
    }
}


int fetchRand(int N){
    return (rand() % N );
}


void printRoomStats(struct Room r){
    printf("xPos = %3d || yPos = %3d   ||  xSize = %3d || ySize = %3d  ||  xItter = %3d || yItter = %3d\n", r.xPos, r.yPos, r.xSize, r.ySize, r.xItter, r.yItter);
}


void makeRoom(struct Room r){
    for(int i = 0; i < r.ySize; i++){
        for(int j = 0; j < r.xSize; j++){
            printf("*");
        }
        printf("\n");
    }
}


void createRooms(){
    for(int i = 0; i < 5; i++){
        int ret = False;
        do{
            rooms[i].xPos = fetchRand(80);
            rooms[i].yPos = fetchRand(21);
            rooms[i].xSize = fetchRand(10);
            rooms[i].ySize = fetchRand(10);
            rooms[i].xItter = rooms[i].xPos + rooms[i].xSize;
            rooms[i].yItter = rooms[i].yPos + rooms[i].ySize;

            if(rooms[i].xSize > 3 && 
                rooms[i].ySize > 2 && 
                (rooms[i].xPos + rooms[i].xSize) < 80 && 
                (rooms[i].yPos + rooms[i].ySize) < 21 && 
                rooms[i].xPos > 1 && 
                rooms[i].yPos > 1
            ){
                ret = True;
        }

        } while (ret != True);
    }
}

void checkCollision(){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            //do nothing...
        }
    }
}
