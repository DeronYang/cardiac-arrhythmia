#include <stdio.h>
#include <stdlib.h>
#include "../extract/median.h"
#define LENGTH  10
int main() {
    char *fpath = "./orig_samp.log";
    FILE *fid = fopen(fpath, "rb");
    if (fid == NULL)
        return -1;
    double *orig_samp = (double *) malloc(sizeof(double) * LENGTH);
    for (int i = 0; i < LENGTH; i++) {
        fscanf(fid, "%lf", &orig_samp[i]);
    }

    fclose(fid);
    double *median_flt_samp_200ms = medfilt1(orig_samp, LENGTH, 4);
    for ( int i = 0; i < LENGTH; i++) {
        printf("%d : %f\n", i, median_flt_samp_200ms[i]);
    }
//    double *median_flt_samp_600ms = medianFilter(median_flt_samp_200ms, LENGTH,
//            108);

    return 0;
}
