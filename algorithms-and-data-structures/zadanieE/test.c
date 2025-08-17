#include <stdio.h>


int main(){

    double number = -3423.234123;

    number = (number << 1) >> 1;


    printf("%f\n", number);

    return 0;
}