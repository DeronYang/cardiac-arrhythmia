#include <jni.h>
#include <string.h>
#include "../src/log.h"
#include "../src/predict.h"
#include "../src/train.h"

namespace example {


static jint doClassification(JNIEnv *env, jobject obj, jint beg, jint end,
        jintArray labelsArr ,jdoubleArray probsArr, jstring modelFiles) {

	jboolean isCopy;
	const char *modelFile = env->GetStringUTFChars(modelFiles, &isCopy);
    int *labels = env->GetIntArrayElements(labelsArr, NULL);
    double *probs = env->GetDoubleArrayElements(probsArr, NULL);

    int r = predict(beg, end, labels, probs, modelFile);

    env->ReleaseIntArrayElements(labelsArr, labels, 0);
    env->ReleaseDoubleArrayElements(probsArr, probs, 0);
    env->ReleaseStringUTFChars(modelFiles, modelFile);
    return r;
}

static JNINativeMethod sMethods[] = {
	/* name, signature, funcPtr */
    {"doClassificationNative", "(II[I[DLjava/lang/String;)I",
                (void*)doClassification},

};

static int jniRegisterNativeMethods(JNIEnv *env, const char *className,
        JNINativeMethod* Methods, int numMethods) {
	LOGD("jniRegisterNativeMethods");
    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, Methods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}


int register_Signal(JNIEnv *env) {
	LOGD("register_Signal");
    return jniRegisterNativeMethods(env, "edu/tju/seie/cardiographclassification/CardiographClassficationActivity",
            sMethods, sizeof(sMethods) / sizeof(sMethods[0]));
}

}
