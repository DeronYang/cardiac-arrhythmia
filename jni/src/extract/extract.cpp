//remove power-line and high-frequency noise with low pass filter
//12-tap low pass filter with cut-off frequency 35Hz/180Hz
//created by matlab
#include <stdio.h>
#include <stdlib.h>
#include "../QRS/processing.h"
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
    int samp_len = 7200;
    samp_pre = (double *) malloc(samp_len * sizeof(double));
    const char *fp_samp_path = "./my_samp_pre.log";
    FILE *fid = fopen(fp_samp_path, "rb");
    if (fid == NULL)
        return -1;
    for (i = 0; i < samp_len; i++) {
        fscanf(fid, "%lf", &samp_pre[i]);
    }

    int *R_result = NULL;
    int R_len = 0;
    processing(samp_pre, samp_len, R_result, R_len);

    printf("********QRS***********\nLength: %d\n", R_len);
    for (i = 0; i < R_len; i++) {
        printf("%4d : %4d\n", i + 1, R_result[i]);
    }

    double *samp_med1 = medianFilter(samp_pre, samp_len, 36);
    double *samp_med2 = medianFilter(samp_med1, samp_len, 108);
    double *&samp_cor = samp_med1; //need free
    for (i = 0; i < samp_len; i++) {
        samp_cor[i] = samp_pre[i] - samp_med2[i];
//        printf("%d : %lf\n", i+1,samp_cor[i]);
    }
    free(samp_pre);
    free(samp_med2);

    double *tmp = conv(samp_cor, samp_len, fir_coeff, 12); //不需要释放；
    double *&samp_filt = tmp;
    for (i = 0; i < samp_len; i++) {
        printf("%d : %f\n", i + 1, samp_filt[i]);
    }
    free(samp_cor);

    int Rows = R_len;
    int Cols = 12;
    double **feature = (double **) malloc(Rows * sizeof(double *));
    for (i = 0; i < Rows; i++) {
        feature[i] = (double *) malloc(Cols * sizeof(double));
//        memset(feature[i], 0, Cols * sizeof(double));
    }
    /*
     * Col: 0->11
     * 0: preRR
     * 1: postRR
     * 2->11:Morphology features (QRS feature)
     *
     */
    for(i=1;i<Rows-1;i++)
    {
        feature[i][0] = ((double)(R_result[i] - R_result[i-1])) / 360;
        feature[i][1] = ((double)(R_result[i+1] - R_result[i])) / 360;
        feature[i][2] = samp_filt[R_result[i] - 18];
        feature[i][3] = samp_filt[R_result[i] - 12];
        feature[i][4] = samp_filt[R_result[i] - 6];
        feature[i][5] = samp_filt[R_result[i]];
        feature[i][6] = samp_filt[R_result[i] + 6];
        feature[i][7] = samp_filt[R_result[i] + 12];
        feature[i][8] = samp_filt[R_result[i] + 18];
        feature[i][9] = samp_filt[R_result[i] + 24];
        feature[i][10] = samp_filt[R_result[i] + 30];
        feature[i][11] = samp_filt[R_result[i] + 36];
    }
    /*
     *  关于前后两段波形的边界处理
     */
    for(i=0;i<Cols;i++)
    {
        printf("%d %f\n",i+1,feature[5][i]);
    }
    return 0;
}
