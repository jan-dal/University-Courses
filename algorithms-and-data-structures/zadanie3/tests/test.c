#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void copy(char *src, char *dest, int size);
void copy2(char *src, char *dest, int size);

int main(){

    struct timespec tstart={0,0}, tend={0,0};

    int n = 2000000;
    int m = 1000;

    char *board = malloc(sizeof(char)*n);
    char **copies = malloc(sizeof(char*)*m);

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for(int i=0; i < m; i++){
        copies[i] = malloc(sizeof(char)*n);
        copy2(board, copies[i], n);
    }
   

    clock_gettime(CLOCK_MONOTONIC, &tend);

    printf("%f\n", ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));


    return 0;
}

void copy(char *src, char *dest, int size){
    for(int i=0; i < size; i++){
        dest[i] = src[i];
    }
}

void copy2(char *src, char *dest, int size){
    memcpy(dest, src, size);
}