//
// Created by youjian on 2023/11/30.
//

#include "wmTool.h"
#include "wmLuaDemo.h"
#include "lua/lua.hpp"

#ifdef __ANDROID__
#include <android/log.h>
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, "wmluaDemo", __VA_ARGS__)
#endif

char* wm_strcat(char* dest, const char* src) {
    char *result = static_cast<char *>(malloc(strlen(dest) + strlen(src) + 1));
    if (result == NULL) return "";

    strcpy(result, dest);
    strcat(result, src);

    return result;
}

int wm_print(lua_State *L) {
    int n = lua_gettop(L);
    char *print_s = "";
    for (int i = 1; i <= n; i++) {
        size_t l;
        const char *s = luaL_tolstring(L, i, &l);
        if (i > 1) {
            print_s = wm_strcat(print_s,"\t");
            print_s = wm_strcat(print_s,s);
        } else {
            print_s = wm_strcat(print_s,s);
        }
    }

    lua_pop(L, n);
#ifdef __ANDROID__
    // 采用android日志输出
    LOGD("%s", print_s);
#endif
    return 0;
}

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