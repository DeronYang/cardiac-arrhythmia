#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#define LOG_TAG "PREDICT"
#include "log.h"
#include "predict.h"
#include "./svm/svm-predict.h"


int predict(int beg, int end, int *labels, double* prob_estimates,const char *modelFile) {
    LOGD("Coming into classification2\n");
    return svmpredict(beg, end,  labels, prob_estimates, const char *modelFile);
}
