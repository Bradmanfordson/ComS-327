#include <stdio.h>
#include <stdlib.h>

int tour(int x, int y);
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

int tour(int x, int y){
    count++;
    board[x][y] = count;  // mark start as visited

 /*
    This is very inefficient. Fix this so it's better

    The idea is to get all possible knight moves and if they are valid and possible given the current space, 
    make sure we havent moved to that position yet and if we haven't, then move there.
 */
    for(int r = -2; r < 3; r++){ 
        for(int c = -2; c < 3; c++){
            int row = x + r;
            int col = y + c;

            if( r == 0 || c == 0 || abs(r) == abs(c)){
                continue; // disregard move because it's an invalid knight move
            }
            else {
                if(row > 0 && row <= 5 && col > 0 && col <= 5){ //for each unmarked square (s) reachable by a knights move from start
                    if(board[row][col] == 0){ // the tile has not been hit
                        tour(row, col); // tour(s)
                    }
                }
            }
        }
    }

    board[x][y] = 0; // unmark start


// Not quite sure what to do from here on.
/*
    Not quite sure what to do from here on.
    My thought process is that I need to check if all of the tiles in the board have been hit
    and if they have been then print the board and reset the board to all 0's.

    Getting segmentation faults.


    int temp = 0;
    
    for( int i = 0; i < 5; i++){
        for( int j = 0; j < 5; j++){
            if(board[i][j] != 0){
                temp++;
            }
        }
    }

    // I figure, once the board is full then I need to print the board... 

    if(temp == 25){
        printBoard();
    }
    printf("\n");
    count = 0;

*/
    
    return 0;
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
