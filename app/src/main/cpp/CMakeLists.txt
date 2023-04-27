# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("ui")
FILE(GLOB_RECURSE FILE_INCLUDES Include/*.h*)
include_directories( # 设置全局头文件目录 使其他源码文件可在任意目录<头文件.h>
        Include/
        Include/Imgui/
        )

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(mui SHARED
        # 源文件

        Source/Imgui/imgui.cpp
        Source/Imgui/imgui_draw.cpp
        Source/Imgui/imgui_demo.cpp
        Source/Imgui/imgui_tables.cpp
        Source/Imgui/imgui_widgets.cpp

        Source/Imgui/imgui_impl_android.cpp
        Source/Imgui/imgui_impl_opengl3.cpp

        Source/Imgui_Android_Input.cpp
        Source/EGL.cpp
        Source/MyFile.cpp
        native-lib.cpp


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
        mui
        android
        EGL
        GLESv3
        log
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})