#include <stdio.h>
#include <stdlib.h>
#include "filter.h"
#include "../extract/median.h"
#define LENGTH  7200
int main() {
	const char *fpath = "./orig_samp.log";
    FILE *fid = fopen(fpath, "rb");
    if (fid == NULL)
        return -1;

    double *orig_samp = (double *) malloc(sizeof(double) * LENGTH);
    for (int i = 0; i < LENGTH; i++) {
        fscanf(fid, "%lf", &orig_samp[i]);
    }
    fclose(fid);

    double *median_flt_samp_200ms = medfilt1(orig_samp, LENGTH, 72);
    double *median_flt_samp_600ms = medfilt1(median_flt_samp_200ms, LENGTH,
            216);

    //for reuse the memory
    double *&Diff_samp = median_flt_samp_200ms;
    for(int i = 0; i<LENGTH; i++)
    {
    	Diff_samp[i] = orig_samp[i] - median_flt_samp_600ms[i];
    }
    free(orig_samp);

    //cheby filter coef.get from matlab
    double b1[5] = {0.001730826067031, 0.006923304268125, 0.010384956402187, 0.006923304268125, 0.001730826067031};
    double a1[5] = {1, -3.068124351369139, 3.846290410066840, -2.295024019721810, 0.547574579954518};
    //for reuse the memory;
    double *&pre_data = median_flt_samp_600ms;
    filter(pre_data, NULL, b1, 5, a1, 5, Diff_samp, LENGTH, NULL);
    free(Diff_samp);
    for(int i=0; i<LENGTH; i++)
    {
    	printf("%d : %16.15lf\n",i,pre_data[i]);
    }
    return 0;
}
