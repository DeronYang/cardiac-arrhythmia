/*
 * Copyright (C) 2011 http://www.csie.ntu.edu.tw/~cjlin/libsvm/
 * Ported by likunarmstrong@gmail.com
 */

#ifndef SVM_PREDICT_H
#define SVM_PREDICT_H
int svmpredict(int beg, int end,int *labels, double* prob_estimates, const char *modelFile);
#endif
