#include <stdio.h>
#include <stdlib.h>

void deg(int pos, int size, int *ptr, char *board, char *mask);
int reduce(char *board, char *mask, int n);
int bipartite(int pos, int n, char *board, char *mask);
void nbrs(int pos, int n, int *ptr, char *bipart, char *board, char *mask);

int main(){

    int n;
    int *d;
    int knights = 0;
    char *board, *mask;

    scanf("%d\n", &n);
    board = malloc(sizeof(char)*3*n);
    mask = malloc(sizeof(char)*3*n);

    for(int i=0; i < n; i++){
        scanf("%c%c%c\n", board+3*i, board+3*i+1, board+3*i+2);
        mask[3*i] = '.';
        mask[3*i+1] = '.';
        mask[3*i+2] = '.';
    }

    knights += reduce(board, mask, n);
    for(int j=0; j < 3*n; j++){
        if(board[j] == '.' && mask[j] == '.'){
            knights += bipartite(j, n, board, mask);
        }
    }

    printf("%d\n", knights);
    for(int i=0; i < n; i++){
        printf("%c%c%c\n", board[3*i], board[3*i+1], board[3*i+2]);
    }

    free(mask);
    free(board);
    return 0;
}

void deg(int pos, int size, int *ptr, char *board, char *mask){
    for(int j=0; j < 5; j++){
        ptr[j] = -1;
    }
    if(board[pos] == 'x' || mask[pos] == 'R'){
        return;
    }
    ptr[0] = 0;
    int p[4] = {0};
    if(pos % 3 == 0){
        p[0] = pos + 5;
        p[1] = pos + 7;
        p[2] = pos - 1;
        p[3] = pos - 5;
    } else if(pos % 3 == 1){
        p[0] = pos + 5;
        p[1] = pos + 7;
        p[2] = pos - 5;
        p[3] = pos - 7;
    } else {
        p[0] = pos + 1;
        p[1] = pos + 5;
        p[2] = pos - 5;
        p[3] = pos - 7;
    }

    int ind = 1;
    for(int i=0; i < 4; i++){
        if(p[i] >= 0 && p[i] < size && board[p[i]] == '.' && mask[p[i]] == '.'){
            ptr[0]++;
            ptr[ind] = p[i];
            ind++;
        }
    }
}

int reduce(char *board, char *mask, int n){

    int knights = 0;
    int *ptr = malloc(sizeof(int)*5);

    for(int i=0; i < 3*n; i++){
        deg(i, 3*n, ptr, board, mask);
        if(ptr[0] == 0){
            board[i] = 'S';
            mask[i] = 'R';
            knights++;
        } else if(ptr[0] == 1){
            board[i] = 'S';
            mask[i] = 'R';
            mask[ptr[1]] = 'R';
            knights++;
            if(ptr[1] < i){
                if(ptr[1] - 8 >= -1){
                    i = ptr[1] - 8;
                } else {
                    i = -1;
                }
            } else {
                if(i-8 >= -1){
                    i -= 8;
                } else {
                    i = -1;
                }
            }
        }
    }
    free(ptr);
    return knights;
}

int bipartite(int pos, int n, char *board, char *mask){
    int left = 0;
    int right = 0;
    int *ptr = malloc(sizeof(int)*5);

    char *bipart = malloc(sizeof(char)*3*n);

    int *ql = malloc(sizeof(int)*3*n);
    int *qr = malloc(sizeof(int)*3*n);
    int sl = 0;
    int sr = 0;

    int *bipartL = malloc(sizeof(int)*3*n);
    int *bipartR = malloc(sizeof(int)*3*n);


    ql[sl] = pos;
    sl++;

    while(sl > 0 || sr > 0){
        if(sl > 0){
            for(int j=0; j < sl; j++){
                nbrs(ql[j], n, ptr, bipart, board, mask);
                bipart[ql[j]] = 'L';
                bipartL[j+left] = ql[j];
                if(ptr[0] > 0){
                    for(int i=1; i < ptr[0]+1; i++){
                        qr[sr] = ptr[i];
                        bipart[ptr[i]] = 'Q';
                        sr++;
                    }
                }
            }
            left += sl;
            sl = 0;
        } else {
            for(int j=0; j < sr; j++){
                nbrs(qr[j], n, ptr, bipart, board, mask);
                bipart[qr[j]] = 'R';
                bipartR[j+right] = qr[j];
                if(ptr[0] > 0){
                    for(int i=1; i < ptr[0]+1; i++){
                        ql[sl] = ptr[i];
                        bipart[ptr[i]] = 'Q';
                        sl++;
                    }
                }
            }
            right += sr;
            sr = 0;
        }
    }
    int knights = 0;

    if(left > right){
        knights = left;
        for(int i=0; i < left; i++){
            int p = bipartL[i];
            board[p] = 'S';
            mask[p] = 'R';
        }
        for(int i=0; i < right; i++){
            int p = bipartR[i];
            mask[p] = 'R';
        }
    } else {
        knights = right;
        for(int i=0; i < right; i++){
            int p = bipartR[i];
            board[p] = 'S';
            mask[p] = 'R';
        }
        for(int i=0; i < left; i++){
            int p = bipartL[i];
            mask[p] = 'R';
        }
    }
    free(ptr);
    free(bipart);
    free(ql);
    free(qr);
    free(bipartL);
    free(bipartR);

    return knights;
}

void nbrs(int pos, int n, int *ptr, char *bipart, char *board, char *mask){
    for(int j=0; j < 5; j++){
        ptr[j] = -1;
    }
    ptr[0] = 0;
    int p[4] = {0};
    if(pos % 3 == 0){
        p[0] = pos + 5;
        p[1] = pos + 7;
        p[2] = pos - 1;
        p[3] = pos - 5;
    } else if(pos % 3 == 1){
        p[0] = pos + 5;
        p[1] = pos + 7;
        p[2] = pos - 5;
        p[3] = pos - 7;
    } else {
        p[0] = pos + 1;
        p[1] = pos + 5;
        p[2] = pos - 5;
        p[3] = pos - 7;
    }
    int ind = 1;
    for(int i=0; i < 4; i++){
        if(p[i] >= 0 && p[i] < 3*n && board[p[i]] == '.' && mask[p[i]] == '.' && bipart[p[i]] != 'L' && bipart[p[i]] != 'R' && bipart[p[i]] != 'Q'){
            ptr[0]++;
            ptr[ind] = p[i];
            ind++;
        }
    }
}
