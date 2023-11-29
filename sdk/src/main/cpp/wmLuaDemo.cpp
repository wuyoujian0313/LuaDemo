//
// Created by youjian on 2023/11/29.
//

#include <jni.h>
#include <string>
#include "lua/lua.hpp"


static void jni_main(JNIEnv *env,jclass luaNativeClass,jobject context,jstring luaScript) {

}


// 注册方式加载JNI方法
static JNINativeMethod methods[] = {
        {"JNIMain", "(Landroid/content/Context;Ljava/lang/String;)V", reinterpret_cast<void *>(jni_main)},
};

static int t_class_init(JNIEnv *env) {
    jclass luaNativeCls = env->FindClass("com/wm/lua/luaNativeDemo");
    if (env->RegisterNatives(luaNativeCls, methods, sizeof(methods) / sizeof((methods)[0])) < 0) {
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