#include <stdio.h>

int tour(int x, int y);

int board[5][5];
int count = 0;

int main(int argc, char *argv[]){
    for(int i=0; i<5; i++){
        for(int j = 0; j<5; j++){
            printf("%2d ", ++count);
            //tour(i, j);
        }
        printf("\n");
    }
    tour(1,1);

    return 0;
}

int tour(int x, int y){
    board[x][y] = 1;

    for(int r = -2; r < 3; r++){
        for(int c = -2; c < 3; c++){
            if( r == 0 || c == 0){
                continue;
            }
            else {
                printf("( %d , %d ) \n", r, c );
            }
        }
    }
    
    // mark start as visited

    //for each unmarked square (s) reachable by a knights move from start
        // tour(s
    //unmark start
    return 0;
}

int checkMove(){

    return 0 ;
}



