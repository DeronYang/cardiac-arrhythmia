#include "median.h"
double median(double *window, int nWinLength);
void   qsort(double *data, int n);

double *medianFilter(double *input, int nInLength, int nRadius)
{
	double *output;
	output = (double *)malloc(sizeof(double) * nInLength);
	double *window;
	int nWinLenth = 2 * nRadius + 1;
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
	double rtn;
	
	return rtn;
}