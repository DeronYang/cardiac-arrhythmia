#include "convolution.h"
#include <stdio.h>
#include <stdlib.h>
double sum(double *in, int n);
double *conv(double *a, int aLen, double *b, int bLen)
{
    if(a == NULL || aLen <=0 || b == NULL || bLen <= 0)
        return NULL;

    int cLen = aLen + bLen - 1;
    double *c = (double *)malloc(sizeof(double) * cLen);

    int sLen,lLen;
    double *ptr_short, *ptr_long;
    if(aLen<bLen)
    {
        sLen = aLen;
        lLen = bLen;
        ptr_short = a;
        ptr_long = b;
    }
    else
    {
        sLen = bLen;
        lLen = aLen;
        ptr_short = b;
        ptr_long = a;
    }
    double *tmp = (double *)malloc(sizeof(double) * sLen);

    int ll, ss, cc;
    for(cc = cLen - 1; cc >=0; cc--)
    {
        int index = cLen - cc;
        ll = lLen - index;
        for(int i = 0; i < sLen; i++)
        {
            if(ll < 0 || ll > lLen - 1)
                tmp[i] = 0;
            else
                tmp[i] = ptr_long[ll] * ptr_short[sLen - 1 - i];
            ll++;
        }
        c[cc] = sum(tmp, sLen);
    }

    free(tmp);
    return c;

}
double sum(double *in, int n)
{
    if(in == NULL || n <=0)
        return 0;
    double out = 0;
    for(int i = 0; i < n; i++)
        out += in[i];
    return out;
}

int main()
{
    double a[9] = {1,2,3,4,5,6,7,8,9};
    double b[12] = {-0.0011, 0.0047, 0.0307, 0.0882, 0.1623, 0.2152, 0.2152, 0.1623, 0.0882, 0.0307, 0.0047, -0.0011};
    double *c = conv(a, 9, b, 12);
    for(int i=0;i<20;i++)
        printf("%f\t",c[i]);
    return 0;
}

