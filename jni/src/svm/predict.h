/*
 * Copyright (C) 2011 http://www.csie.ntu.edu.tw/~cjlin/libsvm/
 * Ported by likunarmstrong@gmail.com
 */

#ifndef PREDICT_H
#define PREDICT_H


int predict(int beg, int end, int *labels, double* prob_estimates,const char *modelFile);

#endif
