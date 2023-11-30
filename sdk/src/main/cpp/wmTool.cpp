//
// Created by youjian on 2023/11/30.
//

#include "wmTool.h"
#include "wmLuaDemo.h"


// 工具方法
std::string jstring2string(JNIEnv *env, jstring jstr) {
    if (jstr == nullptr) {
        return "";
    }
    const char *chars = env->GetStringUTFChars(jstr, nullptr);
    jsize len = env->GetStringUTFLength(jstr);
    std::string str(chars, len);
    env->ReleaseStringUTFChars(jstr, chars);
    return str;
}

// 注册方式加载JNI方法
static JNINativeMethod methods[] = {
        {"JNIMain", "(Landroid/content/Context;Ljava/lang/String;)V", reinterpret_cast<void *>(jni_main)},
};

static int t_class_init(JNIEnv *env) {
    jclass luaNativeCls = env->FindClass("com/wm/lua/luaNativeDemo");
    if (env->RegisterNatives(luaNativeCls, methods, sizeof(methods)/sizeof((methods)[0])) < 0) {
        return JNI_ERR;
    }
    return 0;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv* env;
    if (JNI_OK != vm->GetEnv(reinterpret_cast<void**> (&env),JNI_VERSION_1_4)) {
        return JNI_ERR;
    }

    int ret = t_class_init(env);
    if (ret == JNI_ERR) {
        return ret;
    }

    return JNI_VERSION_1_4;
}