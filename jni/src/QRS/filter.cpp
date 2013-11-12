#include "filter.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    double b[] = { 0.0017, 0.0069, 0.0104, 0.0069, 0.0017 };
    double a[5] = { 1, -3.0681, 3.8463, -2.2950, 0.5476 };
    double x[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    double y[9];
    filter(y, NULL, b, 5, a, 5, x, 9, NULL);
    for (int i = 0; i != 9; i++)
        printf("%f\t", y[i]);
    printf("\n");
//    for( i=0; i!=2; i++ )
//    printf("%f ",zf[i]);
    return 0;
}

