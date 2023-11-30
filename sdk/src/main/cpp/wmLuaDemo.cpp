//
// Created by youjian on 2023/11/29.
//

#include <jni.h>
#include <string>
#include "lua/lua.hpp"
#include "wmTool.h"


// 演示Lua调用C函数 Begin
int add(int a, int b) {
    return a+b;
}

int callC_add(lua_State* L) {
    // 先从栈中取出参数，1和2是a和b在栈中的位置
    int a = lua_tonumber(L, 1);
    int b = lua_tonumber(L, 2);
    // 从栈中弹出已经取值的2个变量
    lua_pop(L, 2);
    // 调用add
    int ret = add(a,b);
    // 结果压入栈
    // Lua脚本中取的返回值这里push进去的
    lua_pushnumber(L, ret);
    // 注意，return 1是告知Lua虚拟机返回一个值
    return 1;
}

// 演示Lua调用C函数 End

// 演示C调用Lua函数 Begin
int callLua_add(lua_State*L, int a, int b) {
    // 将Lua函数add从全局栈中取出压入到函数的Lua栈，
    // 这里告诉Lua虚拟机，我们将要调用的Lua函数add
    lua_getglobal(L, "add");

    // Stack: function (add)
    // 将a变量压入栈
    lua_pushnumber(L, a);

    // Stack: function (add), int (a)
    // 将b变量压入栈
    lua_pushnumber(L, b);

    // Stack: function (add), int (a), int (b)
    // 调用lua_pcall，首先从栈顶弹出入参，最后弹出函数
    // 第二个参数告诉Lua虚拟机弹出2个变量
    // 第三个参数告诉Lua虚拟机返回一个值
    lua_pcall(L, 2, 1, 0);

    // Stack: int (return value of add)
    // 从栈中取出 add 的结果
    int result = lua_tointeger(L, -1);
    lua_pop(L, 1);
    // Stack: empty
    return result;
}
// 演示C调用Lua函数 End


// JNI入口函数，用于测试开发使用
void jni_main(JNIEnv *env,jclass nativeClass,jobject context,jstring luaScript) {
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
        return;
    }

    // 调用Lua脚本的add函数
    int ret = callLua_add(L,1,2);

    // 注册满足签名的C函数到Lua虚拟机
    lua_register(L,"callC_add",callC_add);


    // 关闭库
    lua_close(L);
}


