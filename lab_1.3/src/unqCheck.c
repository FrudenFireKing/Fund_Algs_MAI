#include "../include/unqCheck.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

bool unqCheck(double * arr1, int n1, double * arr2, int n2, double epsilon){
    if (n1 != n2) return true;

    for (int i = 0; i < n1; i++) {
        if (fabs(arr1[i] - arr2[i]) > epsilon) {
            return true;
        }
    }
    
    return false;
}