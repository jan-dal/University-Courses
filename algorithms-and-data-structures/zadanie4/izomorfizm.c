#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct node node;
typedef struct edge edge;

struct edge{
    int v1, v2;
};

struct node{
    int *N;
    int deg, lch, pch, code, parent;
    int *pattern;
};

void start();
void isomorphic(node *N1, node *N2, int *leaves1, int *leaves2, int llen1, int llen2, int v);
void radix(node *N, int* leaves, int llen, int ilen, int m);
int load_tree(node *T, int *leaves, int len);
int propagate_up(node *N, int *leaves, int llen);
int give_code(node *N, int *leaves, int llen);
int compare(int *pattern1, int *pattern2);
int comp_level(node *N1, node *N2, int *leaves1, int *leaves2, int llen1);
void find_parent(node *N1, int i);

int main(){
    int n;
    scanf("%d\n", &n);
    while(n > 0){
        start();
        n--;
    }
    return 0;
}

void start(){
    int v = 0;
    int llen1 = 0;
    int llen2 = 0;
    scanf("%d\n", &v);
    node *N1 = malloc(sizeof(node)*v);
    node *N2 = malloc(sizeof(node)*v);
    int *leaves1 = malloc(sizeof(int)*(v-1));
    int *leaves2 = malloc(sizeof(int)*(v-1));

    llen1 = load_tree(N1, leaves1, v);
    llen2 = load_tree(N2, leaves2, v);

    isomorphic(N1, N2, leaves1, leaves2, llen1, llen2, v);

}

int load_tree(node *N, int *leaves, int v){
    int *childnum = malloc(sizeof(int)*v);
    edge *input = malloc(sizeof(edge)*(v-1));
    int a = 0;
    int b = 0;
    int lind = 0;

    for(int i=0; i < v; i++){
        childnum[i] = 0;
    }
    for(int i=0; i < (v-1); i++){
        scanf("%d %d\n", &a, &b);
        a--;
        b--;
        input[i].v1 = a;
        input[i].v2 = b;
        childnum[a]++;
        childnum[b]++;
    }

    for(int i=0; i < v; i++){
        N[i].N = malloc(sizeof(int)*childnum[i]);
        N[i].pattern = NULL;
        N[i].pch = 0;
        N[i].lch = 0;
        N[i].deg = childnum[i];
        if(childnum[i] == 1){
            leaves[lind] = i;
            lind++;
        }
    }
    for(int i=0; i < (v-1); i++){
        a = input[i].v1;
        b = input[i].v2;
        N[a].N[childnum[a]-1] = b;
        N[b].N[childnum[b]-1] = a;
        childnum[a]--;
        childnum[b]--;
    }
    free(input);
    free(childnum);
    return lind;
}

void isomorphic(node *N1, node *N2, int *leaves1, int *leaves2, int llen1, int llen2, int v){
    int parent = 0;
    int child = 0;
    int l1 = 1;
    int l2 = 1;
    int m1 = 1;
    int m2 = 1;

    for(int i=0; i < llen1; i++){
        child = leaves1[i];
        N1[child].code = 0;
        N1[child].pattern = malloc(sizeof(int)*2);
        N1[child].pattern[0] = 1;
        N1[child].pattern[1] = 0;
    }
    for(int i=0; i < llen2; i++){
        child = leaves2[i];
        N2[child].code = 0;
        N2[child].pattern = malloc(sizeof(int)*2);
        N2[child].pattern[0] = 1;
        N2[child].pattern[1] = 0;
    }

    while(v > 2){

        for(int i=0; i < llen1; i++){
            find_parent(N1, leaves1[i]);
            parent = N1[leaves1[i]].parent;
            N1[parent].deg--;
            N1[parent].lch++;
        }
        for(int i=0; i < llen2; i++){
            find_parent(N2, leaves2[i]);
            parent = N2[leaves2[i]].parent;
            N2[parent].deg--;
            N2[parent].lch++;
        }

        radix(N1, leaves1, llen1, l1, m1);
        radix(N2, leaves2, llen2, l2, m2);

        m1 = give_code(N1, leaves1, llen1);
        m2 = give_code(N2, leaves2, llen2);



        if(llen1 != llen2 || !comp_level(N1, N2, leaves1, leaves2, llen1)){
            printf("NIE\n");
            return;
        }

        v -= llen1;

        llen1 = propagate_up(N1, leaves1, llen1);
        llen2 = propagate_up(N2, leaves2, llen2);
    }

    if(llen1 == llen2 && comp_level(N1, N2, leaves1, leaves2, llen1)){
        printf("TAK\n");
        
    } else {
        printf("NIE\n");
    }

}

int propagate_up(node *N, int *leaves, int llen){
    int l = 0;
    int parent = 0;

    for(int i=0; i < llen; i++){
        parent = N[leaves[i]].parent;
        if(N[parent].deg == 1){
            if(N[parent].pattern == NULL){
                N[parent].pattern = malloc(sizeof(int)*(N[parent].lch+1));
                N[parent].pattern[0] = N[parent].lch; 
            }
            N[parent].pattern[N[parent].pch+1] = N[leaves[i]].code; //Assume leaves are sorted by pattern
            N[parent].pch++;
            N[leaves[i]].deg = 0;
            leaves[l] = parent;
            l++;
        }else{
            N[parent].lch = 0;
            N[leaves[i]].deg = 0;
        }
    }
    return l;
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

int give_code(node *N, int *leaves, int llen){
    int code = 0;
    int *pat = N[leaves[0]].pattern;
    int out = 0;

    N[leaves[0]].code = code;

    for(int i=1; i < llen; i++){
        out = compare(pat, N[leaves[i]].pattern);

        if(!out){
            code++;
            pat = N[leaves[i]].pattern;
        }
        N[leaves[i]].code = code;
    }

    return code+1;
}

int compare(int *pattern1, int *pattern2){

    int len1 = pattern1[0];
    int len2 = pattern2[0];

    if(len1 != len2){
        return 0;
    }
    for(int i=1; i < len1; i++){
        if(pattern1[i] != pattern2[i]){
            return 0;
        }
    }
    return 1;
}

int comp_level(node *N1, node *N2, int *leaves1, int *leaves2, int llen1){
    for(int i=0; i < llen1; i++){
        if(N1[leaves1[i]].code != N2[leaves2[i]].code){
            return 0;
        }
    }
    return 1;
}

void find_parent(node *N1, int i){
    for(int j=0; j < N1[i].lch + N1[i].deg + N1[i].pch; j++){
        if(N1[N1[i].N[j]].deg != 0){
            N1[i].parent = N1[i].N[j];
            return;
        }
    }
}