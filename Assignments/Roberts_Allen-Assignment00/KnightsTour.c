#include <stdio.h>
#include <stdlib.h>

void tour(int x, int y);
void zeroBoard();
void printBoard();

int board[5][5];
int count = 0; 

int main(int argc, char *argv[]){

    zeroBoard();

    for(int i=0; i<5; i++){
        for(int j = 0; j<5; j++){
            tour(i, j);
        }
    }
    
    return 0;
}

void tour(int x, int y){
    count++;
    board[x][y] = count;  // mark start as visited

    if ( count == 25 ){
        count = 0;
        printBoard();
    } else { // Check all of the valid Knight moves and check that the knight has not moved there yet.
        if( ( (x + 2) > 0) && ( (x + 2) <= 5) && ( (y + 1) > 0) && ( (y + 1) <= 5) && board[ x + 2 ][ y + 1 ] == 0 ){
            tour(x + 2, y + 1);
        } else if ( ( (x + 2) > 0) && ( (x + 2) <= 5) && ( (y - 1) > 0) && ( (y - 1) <= 5 && board[ x + 2 ][ y - 1 ] == 0 ) ){
            tour(x + 2, y - 1);
        } else if ( ( (x - 2) > 0) && ( (x - 2) <= 5) && ( (y + 1) > 0) && ( (y + 1) <= 5 && board[ x - 2 ][ y + 1 ] == 0 ) ){
            tour(x - 2, y + 1);
        } else if ( ( (x - 2) > 0) && ( (x - 2) <= 5) && ( (y - 1) > 0) && ( (y - 1) <= 5 && board[ x - 2 ][ y - 1 ] == 0 ) ){
            tour(x - 2, y - 1);
        } else if ( ( (x + 1) > 0) && ( (x + 1) <= 5) && ( (y + 2) > 0) && ( (y + 2) <= 5 && board[ x + 1 ][ y + 2 ] == 0 ) ){
            tour(x + 1, y + 2);
        } else if ( ( (x + 1) > 0) && ( (x + 1) <= 5) && ( (y - 2) > 0) && ( (y - 2) <= 5 && board[ x + 1 ][ y - 2 ] == 0 ) ){
            tour(x + 1, y - 2);
        } else if ( ( (x - 1) > 0) && ( (x - 1) <= 5) && ( (y + 2) > 0) && ( (y + 2) <= 5 && board[ x - 1 ][ y + 2 ] == 0 ) ){
            tour(x - 1, y + 2);
        } else if ( ( (x - 1) > 0) && ( (x - 1) <= 5) && ( (y - 2) > 0) && ( (y - 2) <= 5 && board[ x - 1 ][ y - 2 ] == 0 ) ){
            tour(x - 1, y - 2);
        }
    }
    board[x][y] = 0; // unmark start
}

void zeroBoard(){
    for(int i = 0; i < 5; i++){
        for( int j = 0; j < 5; j++){
            board[i][j] = 0;
        }
    }
}

void printBoard(){
    for(int i = 0; i < 5; i++){
        for( int j = 0; j < 5; j++){
            printf("%d ", board[i][j]);
        }
    }
    printf("\n");
    zeroBoard();
}
