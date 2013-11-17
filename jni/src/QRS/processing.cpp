#include <stdio.h>
#include <stdlib.h>
#include "../function/filter.h"
#include "../function/median.h"
#include "../data/int_list.h"
#define LENGTH  7200
#define POSI_LENGTH 2067
#define SAMP_FREQUENCY 360
double max4parts(double *data, int len);
double min4parts(double *data, int len);
double abs(double num);
int round(double num);
double mean(int *data, int n);

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
    for(i=0;i<level;i++)
    	free(swa[i]);
    free(swa);
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

    double **&wpeak = swd;
    for(i=0;i<level;i++)
    	for(j=0;j<points;j++)
    		wpeak[i][j] = ddw[i][j] * swd[i][j];

    for(i=0;i<level;i++)
    {
    	wpeak[i][0] += 1e-10;
    	wpeak[i][points-1]  += 1e-10;
    }


    for(i=0;i<level;i++)
    {
        free(ddw[i]);
    }
    free(ddw);

    const double *Mj4 = wpeak[2];
    double *posi = (double *)malloc(points * sizeof(double));
    double *nega = (double *)malloc(points * sizeof(double));
    memset(posi,0,points * sizeof(double));
    memset(nega,0,points * sizeof(double));
    for(i = 0; i < points; i++)
    {
    	if(Mj4[i] > 0)
    		posi[i] = Mj4[i];
    	else
    		nega[i] = Mj4[i];
    }
    double thposi = max4parts(posi, points);
    double thnega = min4parts(nega, points);
    free(posi);
    free(nega);
    const double thposi_3 = thposi / 3;
    const double thnega_4 = thnega / 4;

    char *interva = (char *)malloc(points * sizeof(char));
    char *cp_posi = (char *)malloc(points * sizeof(char));
    char *cp_nega = (char *)malloc(points * sizeof(char));
    memset(interva, 0, points * sizeof(char));
    memset(cp_posi, 0, points * sizeof(char));
    memset(cp_nega, 0, points * sizeof(char));
    int loca_len = 0;
    for(i=0;i<points;i++)
    {
        if(posi[i] > thposi_3)
        {
            cp_posi[i] = 1;
            loca_len++;
        }
        if(nega[i] < thnega_4)
        {
            cp_nega[i] = -1;
            loca_len++;
        }
        interva[i] = cp_posi[i] + cp_nega[i];
    }
    free(cp_posi);
    free(cp_nega);
    int *loca = (int *)malloc(loca_len * sizeof(int));

    //loca 结果与matlab差1
    for(i=0,j=0;i<points;i++)
    {
        if(interva[i] !=0){
            loca[j] = i;
            j++;
        }
    }

    char *diff = (char *)malloc((loca_len-1) * sizeof(char));

    int loca2_len = 0;
    for(i=0;i<loca_len-1;i++)
    {
        if(loca[i] - loca[i+1] < 80 && loca[i] - loca[i+1] > -80)
        {
            diff[i] = interva[loca[i]] - interva[loca[i+1]];
            if(diff[i] == -2)
                loca2_len++;
        }
        else
            diff[i] = 0;
    }
    int *loca2 = (int *)malloc(loca2_len * sizeof(int));
    for(i=0,j=0;i<loca_len-1;i++)
    {
        if(diff[i] == -2)
        {
            loca2[j] = i;
            j++;
        }

    }
    char *interva2 = (char *)malloc(points * sizeof(char));
    memset(interva2, 0, points * sizeof(char));
    for(i=0;i<loca2_len;i++)
    {
    	interva2[loca[loca2[i]]] = interva[loca[loca2[i]]];
    	interva2[loca[loca2[i]+1]] = interva[loca[loca2[i]+1]];
    }
    char *intervaqs = (char *)malloc(points * sizeof(char));
    memset(intervaqs, 0, points * sizeof(char));
    memcpy(intervaqs, interva2+10,points - 10);



    int mark1,mark2,mark3;
    IntList R_result;
    i = 0;
    j = 0;
    int Rnum = 0;
    while(i<points-1)
    {
    	if(interva2[i]==-1)
    	{
    		mark1 = i;
    		i++;
    		while(i<points-1 && interva2[i]==0)
    		{
    			i++;
    		}
    		mark2 = i;
    		mark3 = round( (abs(Mj4[mark2])*mark1 + abs(Mj4[mark1])*mark2) / (abs(Mj4[mark2]) + abs(Mj4[mark1])) );
    		R_result.insert(mark3-10);
    		//后面执行%count(mark3-10)=300;
    		i = i+60;
    		j = j+1;
    		Rnum = Rnum + 1;
    	}
    	i = i+1;
    }
    //执行count赋值
//    int count_len = R_result.getTail()->mData + 1;
    int count_len = points;
    int *count = (int *)malloc(count_len * sizeof(int));
    memset(count, 0, sizeof(int) * count_len);
    const ListNode *pCur = R_result.getHead();
    int *R_R;
    double RRmean;
    while(pCur != NULL)
    {
    	count[pCur->mData] = 300;
    	pCur = pCur->pNext;
    }

    int num2 = 1;
    while(num2 != 0)
    {
    	num2 = 0;
    	IntList R;
    	for(i=0;i<count_len;i++)
    	{
    		if(count[i] != 0)
    			R.insert(i);
    	}
    	int RR_len = R.getLength() - 1;
    	R_R = (int *)malloc(RR_len * sizeof(int));
    	pCur = R.getHead();
    	for(i=0;i<RR_len;i++)
    	{
    		R_R[i] = pCur->pNext->mData - pCur->mData;
    		pCur = pCur->pNext;
    	}
    	RRmean = mean(R_R, RR_len);
    	free(R_R);
    	pCur = R.getHead();
    	while(pCur->pNext != NULL)
    	{
    		if(pCur->pNext->mData - pCur->mData <= 0.4*RRmean)
    		{
    			num2 = num2 + 1;
    			if(signal[pCur->pNext->mData] > signal[pCur->mData])
    				count[pCur->mData] = 0;
    			else
    				count[pCur->pNext->mData] = 0;
    		}
    		pCur = pCur->pNext;
    	}
    }

    int num1 = 2;
    while(num1>0)
    {
    	num1 = num1 - 1;
    	IntList R;
    	for(i=0;i<count_len;i++)
    	{
    	    if(count[i] != 0)
    	    	R.insert(i);
    	}
    }
//    printf("thposi:%lf\nthnega:%lf\n",thposi,thnega);
//    const char *fp_wpeak[4] = {"wpeak1.log","wpeak2.log","wpeak3.log","wpeak4.log",};
//    for(i=0;i<4;i++)
//    {
//    	fid = fopen(fp_wpeak[i],"w+");
//    	    for(j=0;j<points;j++)
//    	    {
//    	    	fprintf(fid,"%f\n",wpeak[i][j]);
//    	    }
//    	fclose(fid);
//    }

    return 0;
}
double max(double *data, int len)
{
	double max = 0;
	if(data == NULL || len <= 0)
			return max;
	max = data[0];
	for(int i=1;i<len;i++)
	{
		if(data[i]>max)
			max = data[i];
	}
	return max;
}
double max4parts(double *data, int len)
{
	double result = 0;
	if(data == NULL || len <= 0)
		return result;

	result = (max(data,len/4) + max( data+(len/4), (len/4) ) + max( data+2*(len/4), (len/4) ) + max( data+3*(len/4), len-3*(len/4) ))/4;
	return result;
}
double min(double *data, int len)
{
	double min = 0;
	if(data == NULL || len <= 0)
			return min;
	min = data[0];
	for(int i=1;i<len;i++)
	{
		if(data[i]<min)
			min = data[i];
	}
	return min;
}
double min4parts(double *data, int len)
{
	double result = 0;
	if(data == NULL || len <= 0)
		return result;

	result = (min(data,len/4) + min( data+(len/4), (len/4) ) + min( data+2*(len/4), (len/4) ) + min( data+3*(len/4), len-3*(len/4) ))/4;
	return result;
}

double abs(double num)
{
	if(num<0)
		num *= -1;
	return num;
}

int round(double num)
{
	return (int) (num + 0.5);
}

double mean(int *data, int n)
{
	double rt = 0;
	if(data ==NULL || n<1)
		return rt;
	int sum = 0;
	for(int i = 0;i<n;i++)
		sum += data[i];
	return ((double) sum) / n;

}
