#include <stdio.h>
#include <stdlib.h>

int tour(int x, int y);
void printBoard();
void resetBoard();

int board[5][5];
int count = 1; // testing purposes

int main(int argc, char *argv[]){
    resetBoard();

    for(int i=0; i<5; i++){
        for(int j = 0; j<5; j++){
           printf("Hit\n");
            tour(i, j);
        }
    }
    
    return 0;
}

int tour(int x, int y){
    board[x][y] = count;  // mark start as visited
    count++;
    for(int r = -2; r < 3; r++){ 
        for(int c = -2; c < 3; c++){
            int row = x + r;
            int col = y + c;

            if( r == 0 || c == 0 || abs(r) == abs(c)){
                continue; // disregard move because it's an invalid knight move
            }
            else {
                if(row > 0 && row <= 5 && col > 0 && col <= 5){ //for each unmarked square (s) reachable by a knights move from start
                    if(board[row][col] == 0){
                        tour(row, col); // tour(s)
                    }
                }
            }
        }
    }



    board[x][y] = 0; // unmark start

    int temp = 0;
    
    for( int i = 0; i < 5; i++){
        for( int j = 0; j < 5; j++){
            if(board[i][j] != 0){
                temp++;
            }
        }
    }
    if(temp == 24){
        printBoard();
    }
    printf("\n");
    count = 0;
    
    return 0;
}

void resetBoard(){
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
    resetBoard();
}



