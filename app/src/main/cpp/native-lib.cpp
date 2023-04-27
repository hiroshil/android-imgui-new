#include <jni.h>
#include <string>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include "imgui.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include <android/log.h>
#include <android/asset_manager.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "EGL.h"
#include <random>
#include "android/native_window_jni.h"

string            SetFilePath;
ImguiAndroidInput input;
EGL       *mEGL   = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_game_ui_GLES3JNILib_setFilePath(JNIEnv *env, jclass clazz, jstring ph) {
    // TODO: implement setFilePath()
    SetFilePath=env->GetStringUTFChars(ph, JNI_FALSE);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_game_ui_Views_mySurfaceView_init(JNIEnv *env, jclass clazz) {
    // TODO: implement init()
    input.funMshowinit(clazz, env);
}




///SurfaceView生命周期 创建
extern "C"
JNIEXPORT void JNICALL
Java_com_game_ui_GLES3JNILib_surfaceCreate(JNIEnv *env, jclass clazz, jobject surface, jint width,
                                           jint high) {
    // TODO: implement surfaceCreate()
    if (mEGL == nullptr) {
        mEGL = new EGL;
    }
    mEGL->onSurfaceCreate(env, surface, width, high);

    mEGL->setSaveSettingsdir(SetFilePath);
    mEGL->setinput(&input);
    mEGL->ActivityState= true;
}
///SurfaceView生命周期 改变
extern "C"
JNIEXPORT void JNICALL
Java_com_game_ui_GLES3JNILib_surfaceChange(JNIEnv *env, jclass clazz, jint width, jint high) {
    // TODO: implement surfaceChange()
    if (mEGL != nullptr) {
        mEGL->onSurfaceChange(width, high);
        // mData->TouchIsChange = true;
    }
}
///SurfaceView生命周期 销毁
extern "C"
JNIEXPORT void JNICALL
Java_com_game_ui_GLES3JNILib_surfaceDestroyed(JNIEnv *env, jclass clazz) {
    // TODO: implement surfaceDestroyed()
    if (mEGL != nullptr) {
        mEGL->ActivityState= false;
        mEGL->onSurfaceDestroy();
        //EGL->ActivityState= false;
    }
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_game_ui_GLES3JNILib_MotionEventClick(JNIEnv *env, jclass clazz, jint action,
                                              jfloat pos_x, jfloat pos_y) {
    // TODO: implement MotionEventClick()
    if (mEGL== nullptr){
        return false;
    }


    /*
    switch (action) {
        case 1: //按下
           //EGL->io->MouseDown[0] = true;
            EGL->io->AddMousePosEvent(pos_x, pos_y);
            EGL->io->AddMouseButtonEvent(0,0);

            break;
        case 0: //松开
            //EGL->io->MouseDown[0] = false;
            EGL->io->AddMousePosEvent(pos_x, pos_y);
            EGL->io->AddMouseButtonEvent(0,1);

            break;
        case 2: //移动
            EGL->io->AddMousePosEvent(pos_x, pos_y);
            break;
    }

    if (EGL->io->WantCaptureMouse==1)
        return true;
    return false;*/

    // return true;
    return input.InputTouchEvent(action, pos_x, pos_y);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_game_ui_GLES3JNILib_sendKeyEvent_1JNI(JNIEnv *env, jclass clazz, jint action, jint code) {
    // TODO: implement sendKeyEvent_JNI()
    input.InputKey(action, code);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_game_ui_GLES3JNILib_sendDelKey(JNIEnv *env, jclass clazz, jint code) {
    // TODO: implement sendDelKey()
    mEGL->io->AddKeyEvent(ImGuiKey_Backspace,code);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_game_ui_GLES3JNILib_inputcharOnJNI(JNIEnv *env, jclass clazz, jstring msg) {
    // TODO: implement inputcharOnJNI()
    const char *tmp1 = env->GetStringUTFChars(msg, NULL);
    input.addUTF8(tmp1);
    env->ReleaseStringUTFChars(msg, tmp1);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_game_ui_GLES3JNILib_finishComposingText_1JNI(JNIEnv *env, jclass clazz) {
    // TODO: implement finishComposingText_JNI()
    input.Inputio = false;
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_game_ui_GLES3JNILib_Select_1all(JNIEnv *env, jclass clazz) {
    // TODO: implement Select_all()
    return input.JNI_SelectAll();
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_game_ui_GLES3JNILib_Cut(JNIEnv *env, jclass clazz) {
    // TODO: implement Cut()
    return env->NewStringUTF(input.JNI_Cut().c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_game_ui_GLES3JNILib_Copy(JNIEnv *env, jclass clazz) {
    // TODO: implement Copy()
    return env->NewStringUTF(input.JNI_Copy().c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_game_ui_GLES3JNILib_Paste(JNIEnv *env, jclass clazz, jstring data) {
    // TODO: implement Paste()
    const char *tmpbuf = env->GetStringUTFChars(data, NULL);
    input.JNI_Paste(tmpbuf);
    env->ReleaseStringUTFChars(data, tmpbuf);
}