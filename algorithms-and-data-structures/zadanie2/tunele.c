#include <stdio.h>
#include <stdlib.h>

// Buil a max heap in the lexical order of coordinates (x, y) < (x', y') --> x < x' or x = x' and y <= y'
// Sort the heap
// Create a list of squares with the link to the previous squares
// Calculate the paths trawersing from left to right in the sorted list <=> trawersing from top to bottom and left to right on the grid

typedef struct Plac Plac;
typedef struct Link Link;

struct Link{
    int ind;
    Link *next;
};

struct Plac{
    Link *prev;
    int drogi;
    int x;
    int y;
};

Link *create_link(int ind, Link *next);
int sum_paths(Plac * place, Link * link);
int bin_search(Plac* place, int a, int b, int x, int y);
void move_up(int tunele[][4], int i);
void move_down(int tunele[][4], int i, int hsize);
void heapsort(int tunele[][4], int t);
void swap(int tunele[][4], int i, int j);
int parent(int i);
int l_child(int i);
int r_child(int i);

int main(){

    int n, m, t;

    scanf("%d %d %d\n", &m, &n, &t);
    int (*tunele)[4] = malloc(sizeof(*tunele)*t);
    Link **listal = malloc(sizeof(Link)*2*t);
    int l = 0;

    for(int i=0; i < t; i++){
        int x1, x2, y1, y2;

        scanf("%d %d %d %d\n", &x1, &y1, &x2, &y2);
        tunele[i][0] = x1;
        tunele[i][1] = y1;
        tunele[i][2] = x2;
        tunele[i][3] = y2;
        
        move_up(tunele, i);
    }
    heapsort(tunele, t-1);

    Plac *place = malloc(sizeof(Plac)*2*t);

    int newx, newy;
    int cx = -1;
    int cy = -1;
    int pindex = -1;

    for(int tindex=0; tindex < t; tindex++){
        newx = tunele[tindex][0];
        newy = tunele[tindex][1];

        if(newx != cx || newy != cy){
            pindex++;
            cx = newx;
            cy = newy;
            place[pindex].drogi = 0;
            place[pindex].x = cx;
            place[pindex].y = cy;
            place[pindex].prev = create_link(-1, NULL);
            listal[l++] = place[pindex].prev;
        }
    }

    pindex++;
    place[pindex].x = m;
    place[pindex].y = n;
    place[pindex].prev = create_link(-1, NULL);
    listal[l++] = place[pindex].prev;
    place[0].drogi = 1;
    int b = pindex;


    for(int tindex=0; tindex < t; tindex++){
        int sn = bin_search(place, 0, b, tunele[tindex][2], tunele[tindex][3]);

        if(sn != -1){
            int sp = bin_search(place, 0, b, tunele[tindex][0], tunele[tindex][1]);
            place[sn].prev = create_link(sp, place[sn].prev);
            listal[l++] = place[sn].prev;
        }
    }

    for(pindex=1; pindex <= b; pindex++){
        place[pindex].drogi = sum_paths(place, place[pindex].prev);
    }

    printf("%d", place[b].drogi % 999979);
    free(place);
    free(tunele);
    for(int i=l; i >= 0 ; i--){
        free(listal[i]);
    }
    free(listal);
    return 0;
}

int sum_paths(Plac * place, Link *link){
    Link *a = link;
    int suma = 0;
    while(a->ind != -1){
        suma += place[a->ind].drogi % 999979;
        a = a->next;
    }
    return suma % 999979;
}

int bin_search(Plac *place, int a, int b, int x, int y){

    while(a <= b){

        int s = a + ((b-a)/2);
        int sx = place[s].x;
        int sy = place[s].y;

        if(sx == x && sy == y){
            return s;
        }
        if(sx > x || (sx == x && sy > y)){
            b = s-1;
        } else {
            a = s+1;
        }
    }
    return -1;

}

void heapsort(int H[][4], int t){

    for(int i=t; i > 0; i--){
        swap(H, 0, i);
        move_down(H, 0, i-1);
    }

}

void swap(int H[][4], int i, int j){
    int a[4] = {H[i][0], H[i][1], H[i][2], H[i][3]};

    H[i][0] = H[j][0];
    H[i][1] = H[j][1];
    H[i][2] = H[j][2];
    H[i][3] = H[j][3];

    H[j][0] = a[0];
    H[j][1] = a[1];
    H[j][2] = a[2];
    H[j][3] = a[3];
}

void move_down(int H[][4], int i, int hsize){
    int lc, rc;
    int j = -1;
    int k = i;
    while(k != j){
        j = k;
        lc = l_child(j);
        rc = r_child(j);
        if((lc <= hsize) && (H[lc][0] > H[k][0] || (H[lc][0] == H[k][0] && H[lc][1] > H[k][1]))){
            k = lc;
        }
        if((rc <= hsize) && (H[rc][0] > H[k][0] || (H[rc][0] == H[k][0] && H[rc][1] > H[k][1]))){
            k = rc;
        }
        swap(H, j, k);
    }
}

void move_up(int H[][4], int i){

    while(i > 0){
        int j = parent(i);
        if(H[i][0] > H[j][0] || (H[i][0] == H[j][0] && H[i][1] > H[j][1])){
            swap(H, i, j);
            i = j;  
        } else {
            break;
        }
    }
}

int parent(int i){
    return (i-1)/2;
}
int l_child(int i){
    return (2*i)+1;
}
int r_child(int i){
    return (2*i)+2;
}

Link * create_link(int ind, Link *next){
    Link *newlink = malloc(sizeof(Link));
    newlink->ind = ind;
    newlink->next = next;
    return newlink;
}