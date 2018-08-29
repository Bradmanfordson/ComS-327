#include <stdio.h>

int tour(int x, int y);

int board[5][5];
int count = 0;

int main(int argc, char *argv[]){
    for(int i=0; i<5; i++){
        for(int j = 0; j<5; j++){
            printf("%2d ", ++count);
            tour(i, j);
        }
        printf("\n");
    }

    return 0;
}

int tour(int x, int y){
    board[x][y] = 1;

    // mark start as visited

    //for each unmarked square (s) reachable by a knights move from start
        // tour(s
    //unmark start
    return 0;
}



