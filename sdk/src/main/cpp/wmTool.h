//
// Created by youjian on 2023/11/30.
//

#ifndef LUADEMO_WMTOOL_H
#define LUADEMO_WMTOOL_H


#include <jni.h>
#include <string>

std::string jstring2string(JNIEnv *env, jstring jstr);

#endif //LUADEMO_WMTOOL_H
