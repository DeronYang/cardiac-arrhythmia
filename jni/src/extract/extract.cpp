//remove power-line and high-frequency noise with low pass filter
//12-tap low pass filter with cut-off frequency 35Hz/180Hz
//created by matlab
#include <stdio.h>
#include <stdlib.h>
#include "../function/median.h"
#include "../function/convolution.h"
double fir_coeff[12] = { -0.001148116470011, 0.004745771476303,
        0.030661636841520, 0.088238325775048, 0.162261360095697,
        0.215241022281443, 0.215241022281443, 0.162261360095697,
        0.088238325775048, 0.030661636841520, 0.004745771476303,
        -0.001148116470011 };

int main() {
    int i, j;
    double *samp_pre = NULL;
    int samp_len = 100;
    samp_pre = (double *)malloc(samp_len * sizeof(double));
    const char *fp_samp_path = "./my_samp_pre.log";
    FILE *fid = fopen(fp_samp_path, "rb");
    if (fid == NULL)
        return -1;
    for(i=0;i<samp_len;i++)
    {
        fscanf(fid, "%lf", &samp_pre[i]);
    }
    double *samp_med1 = medianFilter(samp_pre, samp_len, 36);
    double *samp_med2 = medianFilter(samp_med1, samp_len, 108);
//    for(i=0;i<100;i++)
//    {
//        printf("%d : %f\n",i+1,samp_med1[i]);
//    }
    double *&samp_cor = samp_med1;//need free
    for (i = 0; i < 100; i++) {
        samp_cor[i] = samp_pre[i] - samp_med2[i];
//        printf("%d : %lf\n", i+1,samp_cor[i]);
    }
    free(samp_pre);
    free(samp_med2);

    double *tmp = conv(samp_cor, samp_len, fir_coeff, 12);//不需要释放；
    double *&samp_filt = tmp;
    for(i=0;i<100;i++)
       {
           printf("%d : %f\n",i+1,samp_filt[i]);
       }
    free(samp_cor);



    return 0;
}
