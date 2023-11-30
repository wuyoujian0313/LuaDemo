# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("sdk")

project(assembler C ASM)

STRING(FIND ${CMAKE_CXX_FLAGS} "-DDEBUG" HAS_DDEBUG)

if (${HAS_DDEBUG} EQUAL -1)
    MESSAGE("NOT DEFINED DEBUG")
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_C_VISIBILITY_PRESET hidden)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

    # tell the linker to transform all the symbols in the static libraries to hidden
    set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS}   -Wl,--exclude-libs,ALL")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wl,--exclude-libs,ALL")

    # 编译的时候，把每个函数作为一个section，每个数据（全局变量等）也作为一个section
    # 如果不加这两个选项，则默认以每个源文件为一个section进行链接，导致只要这个文件中用到了一个函数或数据，所有的函数或数据都会被链接进来
    set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS}   -ffunction-sections -fdata-sections")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
    # 链接的时候，--gc-sections会把没用到的section丢弃掉，最终的可执行文件就只包含用到了的函数和数据
    set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS}   -Wl,--gc-sections")
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wl,--gc-sections")

    # 删除调试符号
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
    # 开启空间优化
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

else()
    MESSAGE("HAS DEFINED DEBUG")
endif ()

MESSAGE(${CMAKE_CXX_FLAGS})
MESSAGE(${CMAKE_C_FLAGS})

include_directories(lua/)

# 加载静态库
add_library(lua_static STATIC IMPORTED)
set_target_properties(lua_static PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/liblua_static.a)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        wmlua_native

        SHARED

        # Provides a relative path to your source file(s).
        wmLuaDemo.cpp
        wmTool.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        wmlua_native

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} android lua_static)
