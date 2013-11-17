#include "filter.h"
#include <stdio.h>
#include <stdlib.h>

void filter(double y[], double zf[], double b[], int lenB, double a[], int lenA,
        const double x[], int lenX, const double zi[]) {
    int i, j;

    if (a[0] == 0)
        throw("a[0] cannot be zero!");
    if (a[0] != 1) {
        for (i = 1; i != lenA; i++)
            a[i] /= a[0];
        for (i = 1; i != lenB; i++)
            b[i] /= a[0];
        a[0] = 1;
    }

    int na = lenA - 1, nb = lenB - 1;
    int len = na > nb ? na : nb;

    bool zi_null = zi == NULL;
    if (zf != NULL) {
        memset(zf, 0, len * sizeof(double));
        if (zi == NULL)
            zi = new double[len]();
    }

    bool zf_null = zf == NULL;
    if (zi != NULL && zf == NULL)
        zf = new double[len]();

    // 1.
    if (zf == NULL && zi == NULL) {

        y[0] = b[0] * x[0];
        for (i = 1; i < lenX; i++) {
            y[i] = 0;
            for (j = 0; j <= nb; j++) {
                if (i - j < 0)
                    break;
                y[i] += b[j] * x[i - j];
            }
            for (j = 1; j <= na; j++) {
                if (i - j < 0)
                    break;
                y[i] -= a[j] * y[i - j];
            }
        }

    }
    // 2.
//    else {
//
//        double *A = new double[len + 1]();
//        memcpy(A, a, lenA * sizeof(double));
//        double *B = new double[len + 1]();
//        memcpy(B, b, lenB * sizeof(double));
//        double *zf_last = new double[len];
//        memcpy(zf_last, zi, len * sizeof(double));
//
//        for (i = 0; i != lenX; i++) {
//            y[i] = b[0] * x[i] + zf_last[0];
//            zf[len - 1] = B[len] * x[i] - A[len] * y[i];
//            for (j = len - 2; j >= 0; j--)
//                zf[j] = B[j + 1] * x[i] + zf_last[j + 1] - A[j + 1] * y[i];
//            memcpy(zf_last, zf, len * sizeof(double));
//        }
//
//        delete[] A;
//        delete[] B;
//        delete[] zf_last;
//
//    }

    return;
}

