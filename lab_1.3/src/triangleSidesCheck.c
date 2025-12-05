#include "../include/triangleSidesCheck.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int triangleSidesCheck(double a, double b, double c, double epsilon) {
    if((a < epsilon) || (b < epsilon) || (c < epsilon)){
        return -1;
    }

    double temp;

    if(b - c > epsilon){
        temp = b;
        b = c;
        c = temp;
    }

    if(a - c > epsilon){
        temp = a;
        a = c;
        c = temp;
    }

    if((a * a + b * b - c * c <= epsilon) && (-epsilon <= a * a + b * b - c * c)){
        return 1;
    }else{
        return 0;
    }
}