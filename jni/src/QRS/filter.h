#ifndef FILTER_H
#define FILTER_H
#pragma once

#include <stdio.h>
#include <memory.h>

/*
 $	Reimplementation of my MATLAB function : [y,zf] = filter(b,a,X,zi)

 filters the data in vector X with the filter described by numerator coefficient vector b and denominator coefficient vector a.
 a(1) might be 1, anyway a(1) cannot be 0.

 if zi isn't NULL and zf isn't NULL, then the function accepts initial conditions, zi, and returns the final conditions, zf, of the filter delays

 $	Inputs,
 a, b, x, zi,
 lenA is the length of array a, lenB is the length of array b, both can be 1.
 Outputs,
 y, zf;

 $	WatchOut,
 1. The length of y[] must agree with the length of x[] !
 2. a[0] cannot be zero !
 3. Length of zf must be max(length(a),length(b))-1 or ZERO !
 4. Length of zi must be the same as zf or ZERO !
 5. Function won't allocate space for outputs y & zf !

 $	Function is written by Yu XianGuo, NUDT ( Aug31, 2012 )
 */

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
//void filter(double *x, int len_x, double *coeff_b, int len_b, double *coeff_a,
//        int len_a, double* zi, int len_zi, double* &filter_x,
//        int& len_filter_x) {
//    double t0 = GetTickCount();
//    len_filter_x = len_x;
//    filter_x = new double[len_x];
//    if (len_a == 1) {
//        for (int m = 0; m < len_x; m++) {
//            filter_x[m] = coeff_b[0] * x[m] + zi[0];
//            for (int i = 1; i < len_b; i++) {
//                zi[i - 1] = coeff_b[i] * x[m] + zi[i]; //-coeff_a[i]*filter_x[m];
//            }
//        }
//    } else {
//        for (int m = 0; m < len_x; m++) {
//            filter_x[m] = coeff_b[0] * x[m] + zi[0];
//            for (int i = 1; i < len_b; i++) {
//                zi[i - 1] = coeff_b[i] * x[m] + zi[i]
//                        - coeff_a[i] * filter_x[m];
//            }
//        }
//    }
//    double t1 = GetTickCount();
//    printf("filter  time- dt= %f ms\n", (t1 - t0));
//}
#endif
/*	验证...................
 1. MATLAB

 b=[0.8147,0.9058,0.1270];
 x=[0.9134,0.6324,0.0975,0.2785,0.5469,0.9575,0.9649,0.1576,0.9706,0.9572];
 [y,zf] = filter(b,1,x);

 2. C++

 double	b[] = { 0.8147, 0.9058, 0.1270 };
 double	a[1] = { 1 };
 double	x[] = { 0.9134, 0.6324, 0.0975, 0.2785, 0.5469, 0.9575, 0.9649, 0.1576, 0.9706, 0.9572 };

 double	zf[2];
 double	y[10];
 yuMatlabFilter( y, zf, b, 3, a, 1, x, 10, NULL );

 for( int i=0; i!=10; i++ )
 printf("%f ",y[i]);
 printf("\n");
 for( int i=0; i!=2; i++ )
 printf("%f ",zf[i]);
 */

/* 本程序对应的MATLAB代码......

 function   [y,zf] = yuFilter(b,a,x,zi)
 % This function reimplement the MATLAB inner function filter().
 % All the parameters accords with the MATLAB inner function filter().
 %
 % Function is written by Yu XianGuo, NUDT ( August 31, 2012 )

 if a(1)==0
 return;
 end
 if a(1)~=1
 for i=2:length(a)
 a(i) = a(i)/a(1);
 end
 for i=2:length(b)
 b(i) = b(i)/a(1);
 end
 a(1) = 1;
 end
 na = length(a)-1;
 nb = length(b) - 1;
 len = max(na,nb); % length of zf

 if nargout==1
 zf = [];
 else
 if nargin<4
 zi = zeros(1,len);
 elseif length(zi)~=len
 return;
 end
 zf = zeros(size(zi));
 zf_last = zi;
 end

 y = zeros(size(x));

 if isempty(zf)

 y(1) = b(1)*x(1);
 for i=2:length(x)
 for j=1:nb+1
 if(i+1-j<1) break; end
 y(i) = y(i) + b(j)*x(i+1-j);
 end
 for j=2:na+1
 if(i+1-j<1) break; end
 y(i) = y(i) - a(j)*x(i+1-j);
 end
 end

 end

 if ~isempty(zf)

 a = [a zeros(1,nb-na)];
 len = length(zf);
 for i=1:length(x)
 y(i) = b(1)*x(i) + zf_last(1);
 zf(len) = b(len+1)*x(i) - a(len+1)*y(i);
 for j=len-1:-1:1
 zf(j) = b(j+1)*x(i) + zf_last(j+1) - a(j+1)*y(i);
 end
 zf_last = zf;
 end

 end


 */
