#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct point point;

struct point{
    int x, y;
};

double distance(double a, double b, double x2, double y2);
void heapsort(point *psetx, int t, int (*comp)(point, point));
void swap(point *psetx, int i, int j);
void move_down(point *psetx, int i, int hsize, int (*comp)(point, point));
void move_up(point *psetx, int i, int (*comp)(point, point));
int parent(int i);
int l_child(int i);
int r_child(int i);
int compx(point a, point b);
int compy(point a, point b);
void buildheap(point *psetx, int n, int (*comp)(point, point));
double cut(point *psetx, point *psety, int a, int b, point *cp);
double connect(point *psety, int a, double c, int gap, double maxdist, point *cp);
void merge(point *psety, int a, int b, int c);

int main(){
    int n = 0, nc = 0, ind = 0, w = 1, last = 0, j = 0;
    scanf("%d\n", &n);
    int bin = (int)(floor(log2(n)+1));
    point *cp = malloc(sizeof(point)*2);
    point *ncp = malloc(sizeof(point)*2);
    point *psetx = malloc(sizeof(point)*n);
    point *psety = malloc(sizeof(point)*n);
    point *closest = malloc(sizeof(point)*bin*2);
    double *dist = malloc(sizeof(double)*bin);
    int *cuts = malloc(sizeof(point)*bin);
    for(int i=0; i < n; i++){
        scanf("%d %d\n", &psetx[i].x, &psetx[i].y);
    }
    heapsort(psetx, n, compx);
    for(int i=0; i < n; i++){
        psety[i].x = psetx[i].x;
        psety[i].y = psetx[i].y;
    }

    while(nc < n){
        if(((n >> ind)&1) == 1){

            nc += w;
            dist[j] = cut(psetx, psety, last, nc-1, cp);
            closest[2*j].x = cp[0].x;
            closest[2*j].y = cp[0].y;
            closest[2*j+1].x = cp[1].x;
            closest[2*j+1].y = cp[1].y;
            cuts[j] = nc;
            last = nc;
            j++;
        }
        w *= 2;
        ind++;
    }

    cuts[j] = n;
    double mindist = dist[0];
    cp[0].x = closest[0].x;
    cp[0].y = closest[0].y;
    cp[1].x = closest[1].x;
    cp[1].y = closest[1].y;
    for(int i=1; i < j; i++){
        if(dist[i] < mindist){
            mindist = dist[i];
            cp[0].x = closest[2*i].x;
            cp[0].y = closest[2*i].y;
            cp[1].x = closest[2*i+1].x;
            cp[1].y = closest[2*i+1].y;
        }
    }

    double newdist = mindist;

    for(int i=0; i < j-1; i++){
        merge(psety, 0, cuts[i], cuts[i+1]-1);

        if(dist[i] > dist[i+1]){
            newdist = connect(psety, 0, fabs(((double)psetx[cuts[i]-1].x + (double)psetx[cuts[i]].x)/2), cuts[i+1], dist[i], ncp);
        } else {
            newdist = connect(psety, 0, fabs(((double)psetx[cuts[i]-1].x + (double)psetx[cuts[i]].x)/2), cuts[i+1], dist[i+1], ncp);
        }
        if(newdist < mindist){
            mindist = newdist;
            cp[0].x = ncp[0].x;
            cp[0].y = ncp[0].y;
            cp[1].x = ncp[1].x;
            cp[1].y = ncp[1].y;
        }
    }
    printf("%d %d\n%d %d\n", cp[0].x, cp[0].y, cp[1].x, cp[1].y);
    free(dist);
    free(cuts);
    free(psety);
    free(psetx);
    free(cp);
    free(ncp);
    return 0;
}

double distance(double x1, double y1, double x2, double y2){
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void heapsort(point *psetx, int t, int (*comp)(point, point)){
    buildheap(psetx, t, comp);
    for(int i=t-1; i > 0; i--){
        swap(psetx, 0, i);
        move_down(psetx, 0, i-1, comp);
    }

}

void buildheap(point *psetx, int n, int (*comp)(point, point)){
    for(int i=(n-1)/2; i >= 0; i--){
        move_down(psetx, i, n-1, comp);
    }
}

void swap(point *psetx, int i, int j){
    int ax = psetx[i].x;
    int ay = psetx[i].y;

    psetx[i].x = psetx[j].x;
    psetx[i].y = psetx[j].y;
    psetx[j].x = ax;
    psetx[j].y = ay;

}

void move_down(point *psetx, int i, int hsize, int (*comp)(point, point)){
    int lc, rc;
    int j = -1;
    int k = i;
    while(k != j){
        j = k;
        lc = l_child(j);
        rc = r_child(j);
        if((lc <= hsize) && comp(psetx[lc], psetx[k])){
            k = lc;
        }
        if((rc <= hsize) && comp(psetx[rc], psetx[k])){
            k = rc;
        }
        swap(psetx, j, k);
    }
}

void move_up(point *psetx, int i, int (*comp)(point, point)){

    while(i > 0){
        int j = parent(i);
        if(comp(psetx[i], psetx[j])){
            swap(psetx, i, j);
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

int compx(point a, point b){
    if(a.x >= b.x){
        return 1;
    }
    return 0;
}

int compy(point a, point b){
    if(a.y >= b.y){
        return 1;
    }
    return 0;
}

double cut(point *psetx, point *psety, int a, int b, point *cp){
    int size = b-a+1;
    int gap = 4;
    double maxdist = 0;
    int j = 0;

    if(size == 1){
        cp[0].x = psetx[0].x;
        cp[0].y = psety[0].y;
        cp[1].x = psetx[1].x;
        cp[1].y = psety[1].y;
        return  distance(psetx[0].x, psetx[0].y, psetx[1].x, psetx[1].y);
    }

    double *dist = malloc(sizeof(double)*size/2);
    point *ncp = malloc(sizeof(point)*2);
    cp[0].x = psetx[a].x;
    cp[0].y = psety[a].y;
    cp[1].x = psetx[a+1].x;
    cp[1].y = psety[a+1].y;
    double mindist = distance(psetx[a].x, psetx[a].y, psetx[a+1].x, psetx[a+1].y);
    for(int i=0; i < size; i += 2){
        dist[i/2] = distance(psetx[a+i].x, psetx[a+i].y, psetx[a+i+1].x, psetx[a+i+1].y);
        if(dist[i/2] < mindist){
            mindist = dist[i/2];
            cp[0].x = psetx[a+i].x;
            cp[0].y = psety[a+i].y;
            cp[1].x = psetx[a+i+1].x;
            cp[1].y = psety[a+i+1].y;
        }
        merge(psety, a+i, a+i+1, a+i+1);
    }

    while(gap <= size){
        j = 0;
        for(int i=0; j < size/gap; i += 2){

            if(dist[i] > dist[i+1]){
                maxdist = dist[i];
            } else {
                maxdist = dist[i+1];
            }
            double c = ((double)psetx[a+gap*j+(gap/2)].x + (double)psetx[a+gap*j-1+(gap/2)].x)/2;
            merge(psety, a+gap*j, a+gap*j+(gap/2), a+gap*(j+1)-1);
            dist[j] = connect(psety, a+gap*j, c, gap, maxdist, ncp);
            if(dist[j] < mindist){
                mindist = dist[j];
                cp[0].x = ncp[0].x;
                cp[0].y = ncp[0].y;
                cp[1].x = ncp[1].x;
                cp[1].y = ncp[1].y;
            }
            j++;
        }
        gap *= 2;
    }

    free(dist); 
    free(ncp);

    return mindist; 
}

double connect(point *psety, int a, double c, int gap, double maxdist, point *cp){
    for(int i=0; i < gap; i++){
        if(fabs((double)psety[a+i].x - c) > maxdist){
            continue;
        }
        int off = 1;
        int count = 0;
        while(count <= 8 && i+off < gap){
            if(fabs((double)psety[a+i+off].x - c) > maxdist){
                off++;
                continue;
            }
            double r = distance(psety[a+i].x, psety[a+i].y, psety[a+i+off].x, psety[a+i+off].y);
            if(maxdist > r){
                cp[0].x = psety[a+i].x;
                cp[0].y = psety[a+i].y;
                cp[1].x = psety[a+i+off].x;
                cp[1].y = psety[a+i+off].y;
                maxdist = r;
            }
            count++;
            off++;
        }
    }
    return maxdist;
}

void merge(point *psety, int a, int b, int c){

    int i = 0;
    int j = 0;
    int ind = 0;

    int sizea = b - a;
    int sizeb = c - b + 1;

    point *pset = malloc(sizeof(point)*(sizea+sizeb));


    while(i < sizea && j < sizeb){
        if(psety[a+i].y < psety[b+j].y){
            pset[ind].x = psety[a+i].x;
            pset[ind].y = psety[a+i].y;
            i++;
        } else {
            pset[ind].x = psety[b+j].x;
            pset[ind].y = psety[b+j].y;
            j++;
        }
        ind++;
    }
    if(i < sizea){
        for(; i < sizea; i++){
            pset[ind].x = psety[a+i].x;
            pset[ind].y = psety[a+i].y;
            ind++;
        }
    } else if(j < sizeb){
        for(; j < sizeb; j++){
            pset[ind].x = psety[b+j].x;
            pset[ind].y = psety[b+j].y;
            ind++;
        }
    }

    for(int k=0; k < sizea+sizeb; k++){
        psety[a+k].x = pset[k].x;
        psety[a+k].y = pset[k].y;
    }

    free(pset);
}