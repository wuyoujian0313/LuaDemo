//
// Created by youjian on 2023/11/30.
//

#ifndef LUADEMO_WMTOOL_H
#define LUADEMO_WMTOOL_H

#include "lua/lua.hpp"
#include <jni.h>
#include <string>

std::string jstring2string(JNIEnv *env, jstring jstr);
int wm_print(lua_State *L);

#endif //LUADEMO_WMTOOL_H
