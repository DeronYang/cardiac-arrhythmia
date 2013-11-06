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

struct svm_model* model;

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


int predict3(int beg, int end, int *labels,double *prob_estimates)
{

	int width = 12;
	int len = end - beg + 1;
//	struct svm_model *model = svm_load_model("/mnt/sdcard/ECG/model.dat");
	struct svm_test_data *test_data = svm_load_test_data(
			"/mnt/sdcard/ECG/SVM2/TestData105.dat", beg, end, width);
	struct svm_result *groundtruth = svm_load_result(
			"/mnt/sdcard/ECG/SVM2/groundtruth105.dat", beg, end);
//	struct svm_result *group = svm_load_result(
//				"/mnt/sdcard/group.dat", begin, end);

	int *my_labels = (int*) malloc(sizeof(int) * len);
	LOGD("My Result2");
	int nright = 0, nwrong = 0;

//	FILE *fp = fopen("/mnt/sdcard/correct", "w");
//	if (fp == NULL)
//		return -1;

	for (int i = 0; i < len; i++) {
		my_labels[i] = my_svm_predict(model, test_data->data[i]);
		labels[i] = my_labels[i];
		LOGD("%d    %d     %d",i+1,my_labels[i],groundtruth->result[i]);
//		fprintf(fp,"%d\n",my_labels[i]);
		if (groundtruth->result[i] == my_labels[i]) {
//			fprintf(fp,"%d\n",i+1);
			nright++;
		} else {
//			LOGD("%d  truth: %d   my_labels: %d", i, groundtruth->result[i], my_labels[i]);
			nwrong++;
		}
	}


//	if (ferror(fp) != 0 || fclose(fp) != 0)
//		return -1;

//	LOGD("Right: %d", nright);
//	LOGD("Wrong: %d", nwrong);
	double ratio = (double) nright / (nright + nwrong);
	LOGD("Ratio: %g", ratio);
//	LOGD("My Result2");
//	int nright = 0, nwrong = 0;
//	for (int i = 0; i < len; i++) {
////		my_labels[i] = my_svm_predict(model, x);
//		if (groundtruth->result[i] == group->result[i]) {
//			nright++;
//		} else {
//			nwrong++;
//		}
//	}
//	LOGD("Right: %d", nright);
//	LOGD("Wrong: %d", nwrong);
//	double ratio = (double) nright / (nright + nwrong);
//	LOGD("Ratio: %g", ratio);
//	LOGD("My Result3");
//	nright = 0, nwrong = 0;
//	for (int i = 0; i < len; i++) {
//		if (group->result[i] == my_labels[i]) {
//			nright++;
//		} else {
//			nwrong++;
//		}
//	}
//	LOGD("Right: %d", nright);
//	LOGD("Wrong: %d", nwrong);
//	ratio = (double) nright / (nright + nwrong);
//	LOGD("Ratio: %g", ratio);
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
	x = (struct svm_node *) malloc(max_nr_attr * sizeof(struct svm_node));

	if (svm_check_probability_model(model) != 0)
	    LOGD("Model supports probability estimates, but disabled in prediction.\n");

	int r = predict3(beg, end,labels, prob_estimates);
	svm_free_and_destroy_model(&model);
	free(x);
	free(line);
	return r;
}
