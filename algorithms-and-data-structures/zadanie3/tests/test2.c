#include <stdio.h>
#include <stdlib.h>

int sols(char col);
int isValid(char *board, int bindex, char *newcol, int j);

int main(){
    char board[2] = {0, 4};
    char col[1] = {4};
    for(int i=0; i < 1; i++){
        printf("Sol %d", isValid(board, 0, col, 0));
        printf("\n %d", sols(7));
    }

    return 0;
}

int isValid(char *board, int bindex, char *newcol, int j){
    // Input : 2 column configuration, 1 new added column
    // Out : True if knights on added column don't attack 2
    ///////  knights on 2 previous columns

    char consider1 = board[bindex];
    char consider2 = board[bindex+1];
    char adding = newcol[j];

    char a[3] = {0};
    char b[3] = {0};



    for(int i=2; i >= 0; i--){
        if(consider1 % 2 == 1){
            a[i] = 1;
        }
        if(consider2 % 2 == 1){
            b[i] = 1;
        }
        consider1 = consider1 >> 1;
        consider2 = consider2 >> 1;
    }
    printf("%d %d %d \n", a[0], a[1], a[2]);
    printf("%d %d %d \n", b[0], b[1], b[2]);
    if(adding % 2 == 1 && (a[1] == 1 || b[0] == 1)){
        return 0;
    }
    adding = adding >> 1;
    if(adding % 2 == 1 && (a[0] == 1 || a[2] == 1)){
        return 0;
    }
    adding = adding >> 1;
    if(adding % 2 == 1 && (a[1] == 1 || b[2] == 1)){
        return 0;
    }

    return 1;
}
int sols(char col){
    int s = 0;

    for(int i=0; i < 3; i++){
        if(col % 2 == 1){
            s++;
        }
        col = col >> 1;
    }
    return s;
}