#include "median.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
double median(double *window, int nWinLength);
void   qsort(double *data, int n);

//used in extract_Wu.m
double *medianFilter(double *input, int nInLength, int nRadius)
{
	double *output;
	output = (double *)malloc(sizeof(double) * nInLength);
	double *window;
	int nWinLength = 2 * nRadius + 1;
	window = (double *)malloc(sizeof(double) * nWinLength);

	for(int i = 0; i < nInLength; i++ )
	{
		int j=0,k=0,kk=0;
		if(i - nRadius < 0)
			for(j = 0; j < nRadius - i; j++)
				window[j] = input[0];
//		if(i + nRadius > nInLength - 1)
//			for(k = nWinLength - 1, kk = 0; k >= i + nRadius; k--,kk++)
//				window[k] = input[nInLength - 1];
		if(i + nRadius > nInLength - 1)
		{
		    kk = i + nRadius - nInLength + 1;
		    int m=0;
		    for(k = nWinLength - 1;m<kk; k--,m++)
		        window[k] = input[nInLength - 1];
		}

		memcpy(window + j, input + i - nRadius + j, (nWinLength - j - kk) * sizeof(double));
		if(i==64)
		{
		    printf("j %d\n",j);
		    printf("kk %d\n",kk);
		    for(int z = 0; z<nWinLength;z++)
		    {
		        printf("%d : %f\n",z+1,window[z]);
		    }
		}
		output[i] = median(window, nWinLength);
	}
	free(window);
	return output;
}
// matlab lib function
double *medfilt1(double *x, int xLen, int n)
{
	if(x == NULL || xLen <= 0 || n <=0)
		return NULL;
	int m = n >> 1;

	double *y = (double *)malloc(sizeof(double) * xLen);
	double *window = (double *)malloc(sizeof(double) * n);
	for(int i = 0; i < xLen; i++)
	{

		for(int j = 0; j < n; j++)
		{
			int index = i + j - m;
			if(index < 0 || index > xLen - 1)
				window[j] = 0;
			else
				window[j] = x[index];

		}

		y[i] = median(window, n);
	}
	return y;
}


double median(double *window, int nWinLength)
{
	if(nWinLength < 2) 
		return  0;
	double rtn;
	int half = nWinLength >> 1;
	qsort(window, nWinLength);
	if(nWinLength % 2 == 0 )
	{
		rtn = (window[half] + window[half - 1]) / 2;
	}
	else
	{
		rtn = window[half];
	}
	
	return rtn;
}

void swap(double *a, double *b)
{
	double tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(double *data, int n)
{
	double pivot = data[n-1];
	int small = -1;
	for(int i = 0; i < n - 1; i++)
	{
		if(data[i] < pivot)
		{
			++small;
			if(small != i)
				swap(&data[i], &data[small]);
		}
	}
	++small;
	swap(&data[small], &data[n-1]);
	return small;
}
void qsort(double *data, int n)
{
	if(n <= 1)
		return;
		
	int index = partition(data, n);
	if(index > 0)
		qsort(data, index);
	if(index < n - 1)
		qsort(data + index + 1, n - index -1);
}


