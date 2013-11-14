#include <stdio.h>
#include <stdlib.h>
#include "filter.h"
#include "../extract/median.h"
#define LENGTH  7200
#define POSI_LENGTH 2067
#define SAMP_FREQUENCY 360
int main() {
	int i, j;
	const char *fp_samp_path = "./orig_samp.log";
    FILE *fid = fopen(fp_samp_path, "rb");
    if (fid == NULL)
        return -1;

    double *orig_samp = (double *) malloc(sizeof(double) * LENGTH);
    for (i = 0; i < LENGTH; i++) {
        fscanf(fid, "%lf", &orig_samp[i]);
    }
    fclose(fid);
    fid = NULL;
    double *median_flt_samp_200ms = medfilt1(orig_samp, LENGTH, 72);
    double *median_flt_samp_600ms = medfilt1(median_flt_samp_200ms, LENGTH,
            216);

    //for reuse the memory
    double *&Diff_samp = median_flt_samp_200ms;
    for(i = 0; i<LENGTH; i++)
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


    const char *fp_posi_path = "./orig_posi.log";
    fid = fopen(fp_posi_path, "rb");
    if (fid == NULL)
    	return -1;

    int *orig_posi = (int *)malloc(sizeof(int) * POSI_LENGTH);
    for (i = 0; i < POSI_LENGTH; i++) {
            fscanf(fid, "%d", &orig_posi[i]);
    }

    double *rr_interval = (double *)malloc(sizeof(double) * POSI_LENGTH);

    for(i = 0 ; i < POSI_LENGTH-1; i++)
    {
    	rr_interval[i] = ((double)(orig_posi[i+1] - orig_posi[i])) / 360;
    }
    rr_interval[POSI_LENGTH - 1] = 0;
    free(orig_posi);
    //to do load labels

    //算小波系数和尺度系数
    int points = LENGTH;
    int level = 4;
    int sr = SAMP_FREQUENCY;
    double *&signal = pre_data;
    double **swa = (double **)malloc(sizeof(double *) * level);
    double **swd = (double **)malloc(sizeof(double *) * level);
    for(i = 0; i<level; i++)
    {
    	swa[i] = (double *)malloc(sizeof(double) * points);
    	memset(swa[i], 0, sizeof(double) * points);
    	swd[i] = (double *)malloc(sizeof(double) * points);
    	memset(swd[i], 0, sizeof(double) * points);
    }

    for(j = 0; j < points - 3; j++)
    {
    	swa[0][j+3] =  0.25 * signal[j+3] + 0.75 * signal[j+2] + 0.75 * signal[j+1] + 0.25 * signal[j];
    	swd[0][j+3] = -0.25 * signal[j+3] - 0.75 * signal[j+2] + 0.75 * signal[j+1] + 0.25 * signal[j];
    }

    int exp2 = 1;
    for(i = 1; i < level; i++)
    {
    	exp2 *= 2;
    	for(int j = 0; j < points - 24; j++)
    	{
    		swa[i][j+24] =  0.25 * swa[i-1][j+24] + 0.75 * swa[i-1][j+24-exp2] + 0.75 * swa[i-1][j+24-exp2*2] + 0.25 * swa[i-1][j+24-exp2*3];
    		swd[i][j+24] = -0.25 * swa[i-1][j+24] - 0.75 * swa[i-1][j+24-exp2] + 0.75 * swa[i-1][j+24-exp2*2] + 0.25 * swa[i-1][j+24-exp2*3];
//    		printf("(%d,%d) %f\n",i+1,j+25, swd[i][j+24]);
    	}
    }
    char *pdw = (char *)malloc((points-1) * sizeof(char));
    char **pddw = (char **)malloc(level * sizeof(char *));
    for(i = 0; i < level; i++)
    {
    	pddw[i] = (char *)malloc(points * sizeof(char));
        memset(pddw[i],0, points * sizeof(char));
    }

    for(i = 0; i<level; i++)
    {
        memset(pdw, 0, (points - 1) * sizeof(char));
    	for(j = 1; j<points;j++)
    	{
    	    if(swd[i][j] > 0 && swd[i][j] > swd[i][j-1])
    	    	pdw[j-1] = 1;
    	}

    	for(j = 1; j < points - 1; j++)
    	{
    	    if(pdw[j-1] > pdw[j])
    	    	pddw[i][j] = 1;
    	}

    }
    char *&ndw = pdw;
    char **nddw = (char **)malloc(level * sizeof(char *));
    for(i = 0; i < level; i++)
    {
    	nddw[i] = (char *)malloc(points * sizeof(char));
        memset(nddw[i],0, points * sizeof(char));
    }

    for(i = 0; i<level; i++)
    {
        memset(ndw, 0, (points - 1) * sizeof(char));
        for(j = 1; j<points;j++)
        {
        	if(swd[i][j] < 0 && swd[i][j] < swd[i][j-1])
        	    ndw[j-1] = 1;
        }

        for(j = 1; j < points - 1; j++)
        {
        	if(ndw[j-1] > ndw[j])
        	    nddw[i][j] = 1;
        }

    }
    free(ndw);
    char **&ddw = pddw;
    for(i=0;i<level;i++)
    {
    	for(j=1;j<points-1;j++)
    	{
    		ddw[i][j] = pddw[i][j] || nddw[i][j];
    	}
    	ddw[i][0] = ddw[i][points-1] = 1;
    }

    for(i=0;i<level;i++)
    {
    	free(nddw[i]);
    }
    free(nddw);

    const char *fp_ddw[4] = {"ddw1.log","ddw2.log","ddw3.log","ddw4.log",};
    for(i=0;i<4;i++)
    {
    	fid = fopen(fp_ddw[i],"w+");
    	    for(j=0;j<points;j++)
    	    {
    	    	if(ddw[i][j]==1)
    	    	fprintf(fid,"%d\n",j+1);
    	    }
    	    fclose(fid);
    }
    return 0;
}
