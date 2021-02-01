#include <string.h>
#include <jni.h>

extern "C" {
    JNIEXPORT jstring JNICALL Java_com_atakmap_android_plugintemplate_HelloJni_stringFromJNI( JNIEnv* env, jobject thiz )
    {
        return env->NewStringUTF("Hi Main Bridge from CPP.");
    }
}
