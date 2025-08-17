#include <stdio.h>
#include <stdlib.h>

void convert(int i, char *board, char t[3]);
void createperm(char **cperm, char *cpermsize);
char *colperm(char col, char **cperm, char *cpermsize);
int isValid(char *boards, int bindex, char *newcols, int j);
int reduceTobest(int **pending, int pend);
void mergesort(int **pending, int l, int p, int key);
void merge(int **list, int l, int mid, int r, int key);
int sols(char col);

int main(){

    int n = 0;
    int nboards = 1; 

    int *solutions;
    int *newsolutions;
    int **pending;
    char **boards;
    char **cperm;
    char *cpermsize;
    char *board;
    char *newcols;
    char column;
    char t[3];
    scanf("%d\n", &n);

    board = malloc(sizeof(char)*(n+2));
    boards = malloc(sizeof(char*)*36);
    cperm = malloc(sizeof(char*)*8);
    cpermsize = malloc(sizeof(char)*8);
    solutions = malloc(sizeof(int)*36);
    newsolutions = malloc(sizeof(int)*36);
    pending = malloc(sizeof(int*)*36);


    createperm(cperm, cpermsize);
    board[0] = 56;
    board[1] = 56;

    for(int i=2; i < n+2; i++){
        scanf("%c%c%c\n", t, t+1, t+2);
        convert(i, board, t);
    }

    for(int i=0; i < 36; i++){
        pending[i] = malloc(sizeof(int)*2);
        boards[i] = malloc(sizeof(char)*(n+2)*2);
        pending[i][1] = -2;
        solutions[i] = 0;
        newsolutions[i] = 0;
    }
    
    boards[0][0] = 0;
    boards[0][1] = 0;
    solutions[0] = 0;

    int bindex = 0;

    ////////////////////////////////////////////////
    ////////////////////////////////////////////////

    for(int i=2; i < n+2; i++){
        int lboards = 0;

        char permsize = cpermsize[board[i]/8];
        newcols = cperm[board[i]/8];
        // For every permutation of current column create best solutions + sort
        
        for(int j=0; j < permsize; j++){
            int pend = 0;
            for(int k=0; k < nboards; k++){

                if(isValid(boards[k], bindex, newcols, j)){
                    pending[pend][0] = (int)boards[k][bindex+1];
                    int s = solutions[k] + sols(newcols[j]);
                    pending[pend][1] = s;
                    pend++;
                }
            }

            int size = reduceTobest(pending, pend);

            for(int p = 0; p < size; p++){
                boards[p+lboards][bindex+2] = pending[p][0];
                boards[p+lboards][bindex+3] = newcols[j];
                newsolutions[p+lboards] = pending[p][1];
            }
            lboards += size;


        }
        for(int r=0; r < lboards; r++){
            solutions[r] = newsolutions[r];
        }

        bindex += 2;
        nboards = lboards;
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////

    int maxsol = 0;
    int ind = 0;
    for(int i=0; i < nboards; i++){
        if(solutions[i] > maxsol){
            ind = i;
            maxsol = solutions[i];
        }
    }

    int place = n+1;
    int j = bindex + 1;

    while(place >= 2){
        char col[1] = {boards[ind][j]};
        board[place] = col[0];
        char find = boards[ind][j-1];
        int newindex = 0;
        while(1){
            if(find == boards[newindex][j-2] && isValid(boards[newindex], j-3, col, 0)){
                break;
            }
            newindex++;
        }
        ind = newindex;
        place--;
        j -= 2;
    }

    printf("%d\n", maxsol);


    for(int i=0; i < 8; i++){
        free(cperm[i]);
    }

    for(int i=0; i < 36; i++){
        free(boards[i]);
    }
    free(board);
    free(solutions);
    free(boards);
    free(cperm);
    free(cpermsize);
    return 0;
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

void mergesort(int **list, int l, int p, int key){
    if(l >= p){
        return;
    }

    int mid = (p+l)/2;
    mergesort(list, l, mid, key);
    mergesort(list, mid+1, p, key);
    merge(list, l, mid, p, key);
}

void merge(int **list, int l, int mid, int r, int key){

    int ls = mid - l + 1;
    int rs = r - mid;

    int **Larr = malloc(sizeof(int*)*rs);
    int **Rarr = malloc(sizeof(int*)*ls);
    for(int i=0; i < ls; i++){
        Larr[i] = malloc(sizeof(int)*2);
    }
    for(int i=0; i < rs; i++){
        Rarr[i] = malloc(sizeof(int)*2);
    }

    for(int i=0; i < ls; i++){
        Larr[i][0] = list[l+i][0];
        Larr[i][1] = list[l+i][1];
    }
    for(int i=0; i < rs; i++){
        Rarr[i][0] = list[mid + 1 + i][0];
        Rarr[i][1] = list[mid + 1 + i][1];
    }

    int i = 0;
    int j = 0;
    int ind = l;

    while(i < ls && j < rs){
        if(Larr[i][key] > Rarr[j][key]) {
            list[ind][0] = Larr[i][0];
            list[ind][1] = Larr[i][1];
            i++;
        } else {
            list[ind][0] = Rarr[j][0];
            list[ind][1] = Rarr[j][1];
            j++;
        }
        ind++;
    }

    while(i < ls){
        list[ind][0] = Larr[i][0];
        list[ind][1] = Larr[i][1];
        ind++;
        i++;
    }

    while(j < rs){
        list[ind][0] = Rarr[j][0];
        list[ind][1] = Rarr[j][1];
        ind++;
        j++; 
    }

    for(int i=0; i < ls; i++){
        free(Larr[i]);
    }
    for(int i=0; i < rs; i++){
        free(Rarr[i]);
    }
    free(Larr);
    free(Rarr);
}

int reduceTobest(int **pending, int pend){
    // Input : List of pending solutions
    // Given some solutions with one new column config
    // choose the best out of all and also sort
    // all diffrent best solutions.
    // Important to sort because we want to be able to 
    // recreate the whole solution but don't want to remember
    // the actual size of the best compatible column configuration
    // just that it is the best. 
    // Output : Size of sorted pending list
    int size = pend;
    int index = 0;
    int max = 0;
    char consider;

    mergesort(pending, 0, pend-1, 0);

    for(int i=0; i < pend; i++){
        if(pending[i][0] == -1){
            continue;
        }
        consider = pending[i][0];
        max = pending[i][1];
        index = i;
        for(int j=i+1; j < pend; j++){
            if(pending[j][0] != consider){
                i = j-1;
                break;
            }
            if(pending[j][0] == consider && pending[j][1] > max){
                pending[index][0] = -1;
                pending[index][1] = -2;
                index = j;
                max = pending[j][1];
                size--;
            } else if(pending[j][0] == consider && pending[j][1] <= max){
                pending[j][0] = -1;
                pending[j][1] = -2;
                size--;
            }
        }
    }
    mergesort(pending, 0, pend-1, 1);

    return size;
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

void convert(int i, char *board, char t[3]){
    // Input : column (3 chars) in form : '...' {., x, S}
    // Output : encoded column on 8 bytes: 00 | 000 | 000
    // 1) 1 if x                                 1)
    // 2) 1 if S                                       2)

    char num = 0;
    if(t[0] == 'x'){
        num += 32;
    } else if(t[0] == 'S'){
        num += 4;
    }
    if(t[1] == 'x'){
        num += 16;
    } else if(t[1] == 'S'){
        num += 2;
    }
    if(t[2] == 'x'){
        num += 8;
    } else if(t[2] == 'S'){
        num += 1;
    }
    board[i] = num;
}

void createperm(char **cperm, char *cpermsize){

    // Input: empty lists
    // Output: list of possible solutions for a given column (and list of the sizes of these lists)
    // Ex: ... -> s.. , .s. , ..s, ss., s.s, .ss, sss 
    // Encoded -> 1   ,  2  ,  4 ,  3 ,  5 ,  6 ,  7

    cperm[0] = malloc(sizeof(char)*8);
    for(char i=0; i < 8; i++){
        cperm[0][i] = i;
    }
    cperm[1] = malloc(sizeof(char)*4);
    cperm[1][0] = 8;
    cperm[1][1] = 10;
    cperm[1][2] = 12;
    cperm[1][3] = 14;
    cperm[2] = malloc(sizeof(char)*4);
    cperm[2][0] = 16;
    cperm[2][1] = 17;
    cperm[2][2] = 20;
    cperm[2][3] = 21;
    cperm[3] = malloc(sizeof(char)*2);
    cperm[3][0] = 24;
    cperm[3][1] = 28;
    cperm[4] = malloc(sizeof(char)*4);
    cperm[4][0] = 32;
    cperm[4][1] = 33;
    cperm[4][2] = 34;
    cperm[4][3] = 35;
    cperm[5] = malloc(sizeof(char)*2);
    cperm[5][0] = 40;
    cperm[5][1] = 42;
    cperm[6] = malloc(sizeof(char)*2);
    cperm[6][0] = 48;
    cperm[6][1] = 49;
    cperm[7] = malloc(sizeof(char));
    cperm[7][0] = 56;

    cpermsize[0] = 8;
    cpermsize[1] = 4;
    cpermsize[2] = 4;
    cpermsize[3] = 2;
    cpermsize[4] = 4;
    cpermsize[5] = 2;
    cpermsize[6] = 2;
    cpermsize[7] = 1;
}