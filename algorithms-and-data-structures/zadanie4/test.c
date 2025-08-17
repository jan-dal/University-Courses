#include <stdio.h>
#include <stdlib.h>

typedef struct node node;
void radix(node *N, int *leaves, int llen, int ilen, int m);
void print_pattern(int *pattern, int size);

struct node{
    int *N;
    int deg, lch, pch, code;
    int *pattern;
};

int main(){
    int n = 10;
    node *touple = malloc(sizeof(node)*n); 
    int * leaves = malloc(sizeof(int)*n);
    for(int i=0; i < n; i++){
        touple[i].pattern = malloc(sizeof(int)*(i+2));
        leaves[i] = i;
        for(int k=1; k < i+2; k++){
            touple[i].pattern[k] = n-k;
        }
        touple[i].pattern[0] = i+1;
    }
    touple[2].pattern[3] = 20;
    touple[4].pattern[1] = 23;
    touple[5].pattern[3] = 30;
    touple[3].pattern[1] = 2;

    radix(touple, leaves, 10, 10, 31);

    for(int i=0; i < n; i++){
        for(int j=0; j < touple[leaves[i]].pattern[0]; j++){
            printf("%d ", touple[leaves[i]].pattern[j+1]);
        }
        printf("\n");
    }

}

void radix(node *N, int *leaves, int llen, int ilen, int m){
    int index = 0;
    int tlen = 0;
    int p = 0;

    int *mem = malloc(sizeof(int)*m);

    for(int i=0; i < m; i++){
        mem[i] = 0;
    }
    for(int i=0; i < llen; i++){
        for(int j=1; j <= N[leaves[i]].pattern[0]; j++){
            mem[N[leaves[i]].pattern[j]]++;
        }
    }
    mem[0] = llen;

    node ***lists1 = malloc(sizeof(int**)*m);
    node ***lists2 = malloc(sizeof(int**)*m);
    int *ind1 = malloc(sizeof(int)*m);
    int *ind2 = malloc(sizeof(int)*m);
    for(int i=0; i < m; i++){
        lists1[i] = malloc(sizeof(node*)*mem[i]);
        lists2[i] = malloc(sizeof(node*)*mem[i]);
        ind1[i] = 0;
        ind2[i] = 0;
    }
    free(mem);

    for(int k=0; k < llen; k++){
        tlen = N[leaves[k]].pattern[0];
        if(tlen == ilen){
            index = N[leaves[k]].pattern[N[leaves[k]].pattern[0]];
            lists1[index][ind1[index]] = &N[leaves[k]];
            ind1[index]++;
        } else {
            lists1[0][ind1[0]] = &N[leaves[k]];
            ind1[0]++;
        }
    }
    p = 1;

    for(int j=ilen-1; j > 0; j--){
        printf("ITER : %d\n", j);
        if(p == 0){
            for(int k=0; k < m; k++){
                printf("Level : %d\n", k);
                for(int j=0; j < ind2[k]; j++){
                    print_pattern(lists2[k][j]->pattern, lists2[k][j]->pattern[0]);
                }
            }
        } else {
            for(int k=0; k < m; k++){
                printf("Level : %d\n", k);
                for(int j=0; j < ind1[k]; j++){
                    print_pattern(lists1[k][j]->pattern, lists1[k][j]->pattern[0]);
                }
            }
        }

        for(int k=0; k < m; k++){
            if(p == 0){
                for(int i=0; i < ind2[k]; i++){
                    tlen = lists2[k][i]->pattern[0];
                    if(j <= tlen){
                        index = lists2[k][i]->pattern[j];
                        lists1[index][ind1[index]] = lists2[k][i];
                        ind1[index]++;
                    } else {
                        lists1[0][ind1[0]] = lists2[k][i];
                        ind1[0]++;
                    }
                }
                ind2[k] = 0;
            } else {
                for(int i=0; i < ind1[k]; i++){
                    tlen = lists1[k][i]->pattern[0];
                    if(j <= tlen){
                        index = lists1[k][i]->pattern[j];
                        lists2[index][ind2[index]] = lists1[k][i];
                        ind2[index]++;
                    } else {
                        lists2[0][ind2[0]] = lists1[k][i];
                        ind2[0]++;
                    }
                }
                ind1[k] = 0;
            }
        }
        p ^= 1;
    }
    int j = 0;
    if(p % 2 != 0){
        for(int i=0; i < m; i++){
            for(int k=0; k < ind1[i]; k++){
                leaves[j] = lists1[i][k] - N; 
                j++;
            }
        }
    } else {
        for(int i=0; i < m; i++){
            for(int k=0; k < ind2[i]; k++){
                leaves[j] = lists2[i][k] - N;
                j++;
            }
        }
    }

    for(int i=0; i < m; i++){
        free(lists1[i]);
        free(lists2[i]);
    }
    free(lists1);
    free(lists2);
    free(ind1);
    free(ind2);
}

}