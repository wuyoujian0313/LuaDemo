//
// Created by youjian on 2023/11/29.
//

#include <jni.h>
#include <string>
#include "lua/lua.hpp"


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

static void jni_main(JNIEnv *env,jclass luaNativeClass,jobject context,jstring luaScript) {
    // 初始化
    // 创建一个指向lua解释器的指针
    lua_State *L = luaL_newstate();
    // 加载lua标准库
    luaL_openlibs(L);

    std::string lua = jstring2string(env,luaScript);

    // 加载lua脚本
    int result = luaL_loadstring(L, lua.c_str());
    if (result != 0) {
        const char *error = lua_tostring(L, -1);
        lua_close(L);
        return;
    }

    // 调用lua块
    result = lua_pcall(L, 0, 0, 0);
    if (result != 0) {
        const char *error = lua_tostring(L, -1);
        lua_close(L);
        return ;
    }

    // 将入口函数名推入栈
    lua_getglobal(L, "main");

    // 将参数推入栈
    lua_pushlightuserdata(L, context);
    // 调用函数，传入一个参数，留一个返回值
    lua_pcall(L, 1, 1, 0);

    const char *ret = lua_tostring(L, -1);

    lua_pop(L, 1);//返回值出栈
    // 关闭库
    lua_close(L);
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