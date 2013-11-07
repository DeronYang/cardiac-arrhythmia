#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "svm.h"
#define LOG_TAG "PREDICT"
#include "../log.h"
#include "svm-predict.h"

struct svm_node *x;
int max_nr_attr = 64;

static struct svm_model* model;

static char *line = NULL;
static int max_line_len;

static char* readline(FILE *input) {
	int len;

	if (fgets(line, max_line_len, input) == NULL)
		return NULL;

	while (strrchr(line, '\n') == NULL) {
		max_line_len *= 2;
		line = (char *) realloc(line, max_line_len);
		len = (int) strlen(line);
		if (fgets(line + len, max_line_len - len, input) == NULL)
			break;
	}
	return line;
}

void exit_input_error_predict(int line_num) {
	LOGD("Wrong input format at line %d\n", line_num);
	exit(1);
}


int predict(int beg, int end, int *labels,double *prob_estimates)
{

	int width = 12;
	int len = end - beg + 1;
	struct svm_test_data *test_data = svm_load_test_data(
			"/mnt/sdcard/ECG/TestData105.dat", beg, end, width);
	struct svm_result *groundtruth = svm_load_result(
			"/mnt/sdcard/ECG/groundtruth105.dat", beg, end);
	LOGD("My Result2");
	int nright = 0, nwrong = 0;

	for (int i = 0; i < len; i++) {
		labels[i] = my_svm_predict(model, test_data->data[i]);
		LOGD("%d    %d     %d",i+1,labels[i],groundtruth->result[i]);
		if (groundtruth->result[i] == labels[i]) {
			nright++;
		} else {
			nwrong++;
		}
	}
	double ratio = (double) nright / (nright + nwrong);
	LOGD("Ratio: %g", ratio);

	return 0;
}

void exit_with_help_for_predict() {
	LOGD(
			"Usage: svm-predict [options] test_file model_file output_file\n"
					"options:\n"
					"-b probability_estimates: whether to predict probability estimates, 0 or 1 (default 0); for one-class SVM only 0 is supported\n");
	exit(1);
}

int svmpredict(int beg, int end,
		int *labels, double* prob_estimates, const char *modelFile)
{
	if(model == NULL)
	{
		LOGD("model == NULL, Loading...");
		if ((model = svm_load_model(modelFile)) == 0)
		{
			LOGD("can't open model file /mnt/sdcard/ECG/model.dat\n");
			exit(1);
		}
	}

	int r = predict(beg, end,labels, prob_estimates);
//	svm_free_and_destroy_model(&model);
	return r;
}
