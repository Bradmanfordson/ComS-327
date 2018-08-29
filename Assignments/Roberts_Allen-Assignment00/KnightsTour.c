#include <stdio.h>
#include <stdlib.h>

int tour(int x, int y);

int board[5][5];
int count = 0; // testing purposes

int main(int argc, char *argv[]){
    for(int i=0; i<5; i++){
        for(int j = 0; j<5; j++){
            printf("%2d ", ++count); // testing purposes
            //tour(i, j);
        }
        printf("\n");//testing
    }
    printf("\n"); //testing 
    //tour(1,1); //testing

    return 0;
}

int tour(int x, int y){
    board[x][y] = 1;   // mark start as visited

    for(int r = -2; r < 3; r++){ 
        for(int c = -2; c < 3; c++){
            int row = x + r;
            int col = y + c;

            if( r == 0 || c == 0 || abs(r) == abs(c)){
                continue; // disregard move because it's an invalid knight move
            }
            else {
                if( row > 0 && row <= 5 && col > 0 && col <= 5 ){ //for each unmarked square (s) reachable by a knights move from start
                    tour(row, col); // tour(s)
                }
                printf("( %d , %d ) \n", r, c ); // testing purposes
            }
        }
    }
    
    board[x][y] = 0; // unmark start
    return 0;
}

int checkMove(){

    return 0 ;
}



