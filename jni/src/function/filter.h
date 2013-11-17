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

 */
void filter(double y[], double zf[], double b[], int lenB, double a[], int lenA,
        const double x[], int lenX, const double zi[]);

#endif

