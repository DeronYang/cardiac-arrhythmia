#include "median.h"
#include <stdlib.h>
#include <string.h>
double median(double *window, int nWinLength);
void   qsort(double *data, int n);

double *medianFilter(double *input, int nInLength, int nRadius)
{
	double *output;
	output = (double *)malloc(sizeof(double) * nInLength);
	double *window;
	int nWinLength = 2 * nRadius + 1;
	window = (double *)malloc(sizeof(double) * nWinLength);
	
	for(int i = 0; i < nInLength; i++ )
	{
		int j,k,kk;
		if(i - nRadius < 0)
			for(j = 0; j < nRadius - i; j++)
				window[j] = input[0];
		if(i + nRadius > nInLength - 1)
			for(k = nWinLength - 1, kk = 0; k >= i + nRadius; k--,kk++)
				window[k] = input[nInLength - 1];
		memcpy(window + j, input + i - nRadius + j, nWinLength - j - kk);
		
		output[i] = median(window, nWinLength);
	}
	free(window);
	return output;
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
#define N 10
int main()
{
	double data[N] = {2.1, 2.3, 1.4, 2.2, 2.7, 3.1, 3.2, 1.5, 1.6,2.9};
	qsort(data, N);
	return 0;
}