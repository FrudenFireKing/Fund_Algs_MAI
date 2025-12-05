#include "../include/findRoot.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double * findRoot(double a, double b, double c, double epsilon) {
    if (((-epsilon < a) && (a < epsilon)) && ((-epsilon < b) && (b < epsilon))) {
        return NULL;
    }

    double *roots = malloc(3 * sizeof(double));
    if(roots == NULL){
        free(roots);
        return NULL;
    }

    if ((-epsilon < a) && (a < epsilon)) {
        roots[0] = 1;                                //Кол-во корней
        roots[1] = -c / b;
        return roots;
    }

    double d = b * b - 4 * a * c;

    if (d < -epsilon) {
        return NULL;
    } else if (fabs(d) < epsilon) {
        roots[0] = 1;                                //Кол-во корней
        roots[1] = -b / (2 * a);
        return roots;
    } else {
        double x1 = (-b + sqrt(d)) / (2 * a);
        double x2 = (-b - sqrt(d)) / (2 * a);
        if(x1 == x2){
            roots[0] = 1;                                //Кол-во корней
            roots[1] = x1;
        } else {
            roots[0] = 2;                                //Кол-во корней
            roots[1] = x1;
            roots[2] = x2;
        }
        return roots;
    }

    return NULL;
}