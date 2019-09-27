#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
void createRoom(int N);
void makeRoom(struct Room r);
void printRoomStats();
void checkCollision();
void createHalls();


struct Room rooms[5];

int main(int argc, char *argv[]){
    srand(time(NULL));
    createRooms();
    checkCollision();
    CreateBoard();

}


void CreateBoard(){
    int we; int ns;
    for(we = 0; we < 21; we++){
        for(ns = 0; ns < 79; ns++){
            
            if(we == 0 || we == 20){
                printf("_");
            } 

            if((ns == 0 || ns == 78) && we != 0 && we != 20){
                printf("|");
            } 
           
            if(we == (rooms[0].yPos - 1) && ns == (rooms[0].xPos - 2)){
                printf(".");
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
                printf(".");
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
                printf(".");
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
                printf(".");
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
                printf(".");
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
                printf(" ");
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
    int i; int j;
    for(i = 0; i < r.ySize; i++){
        for(j = 0; j < r.xSize; j++){
            printf("*");
        }
        printf("\n");
    }
}


void createRooms(){
    int i;
    for(i = 0; i < 5; i++){
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


void createRoom(int N){
    int ret = False;
        do{
            rooms[N].xPos = fetchRand(80);
            rooms[N].yPos = fetchRand(21);
            rooms[N].xSize = fetchRand(10);
            rooms[N].ySize = fetchRand(10);
            rooms[N].xItter = rooms[N].xPos + rooms[N].xSize;
            rooms[N].yItter = rooms[N].yPos + rooms[N].ySize;

            if(rooms[N].xSize > 3 && 
                rooms[N].ySize > 2 && 
                (rooms[N].xPos + rooms[N].xSize) < 80 && 
                (rooms[N].yPos + rooms[N].ySize) < 21 && 
                rooms[N].xPos > 1 && 
                rooms[N].yPos > 1
            ){
                ret = True;
        }

        } while (ret != True);
}


void checkCollision(){
    // This is EXTREMELY CPU heavy but works sometimes and i'm out of time
    int redo = False;
    int i; int j;
    for(i = 0; i < 5; i++){
        for(j = 0; j < 5; j++){
            if(j == i){
                break;
            } else {
                if(rooms[i].xPos == rooms[j].xPos || rooms[i].yPos == rooms[j].yPos){
                    redo = True;
                } else if( (rooms[i].xPos+1 > rooms[j].xPos && rooms[j].xPos > rooms[i].xItter+1) ||
                           (rooms[i].yPos+1 > rooms[j].yPos && rooms[j].yPos > rooms[i].yItter+1) ){
                    redo = True;
                }
            }

            if(redo == True){
                createRoom(j);
                j = 0;
            }
        }
    }
}

void createHalls(){
    /*
        1. get the (x,y) values foreach room.
        2. take room[0] and connect it to room[1] by printing # in place of the current spaces
            in place of X then once, the X's are the same, do the same with Y
        3. repeat step 2 for room[1] -> room[2], room[2] -> room[3], and room[3]-> room[4]
        4. Add a collision detection algorithm for hallways.
    */
}
