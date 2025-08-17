#include <stdio.h>
int main(){
    int a, b, i;
    scanf("%d %d", &a, &b);
    (b < a) ? (i = b, b = a) : (i = a);
    while(i <= b){
        printf("%d\n", i);
        i++;
    }
    return 0;
}