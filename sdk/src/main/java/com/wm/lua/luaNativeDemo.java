package com.wm.lua;


import android.content.Context;

public class luaNativeDemo {

    static {
        System.loadLibrary("wmlua_native");
    }

    public static native void JNIMain(Context context, String luaScript);
}
