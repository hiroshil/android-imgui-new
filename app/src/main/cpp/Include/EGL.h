//
// Created by admin on 2022/6/10.
//

#ifndef IMGUIMENU_IMGUIEGL_H
#define IMGUIMENU_IMGUIEGL_H

#include "pch.h"

class EGL { ;
    std::condition_variable cond;
    std::mutex              Threadlk;


    FILE                    *numSave       = nullptr;
    FILE                    *ColorSave     = nullptr;
    FILE                    *KeySave       = nullptr;
    //EGL
    EGLDisplay              mEglDisplay;
    EGLSurface              mEglSurface;
    EGLConfig               mEglConfig;
    EGLContext              mEglContext;
    EGLNativeWindowType     SurfaceWin;
    std::thread             *SurfaceThread = nullptr;
    ImguiAndroidInput       *input;
    int                     FPS=90;

    int          surfaceWidthHalf   = 0;
    int          surfaceHighHalf    = 0;
    int          StatusBarHeight    = 0;
    //初始化Egl函数
    int initEgl();


    void clearBuffers();
    int swapBuffers();

    bool ThreadIo;

    void EglThread();


    void Dialog(int type);


    bool  isStyle= false;//

    //Imgui
    int initImgui();

    void imguiMainWinStart();

    void imguiMainWinEnd();


    ImFont   *imFont;
    string           SaveDir;
public:
    ImGuiIO      *io;
    ImGuiStyle   *style;
    ImGuiWindow  *g_window          = nullptr;
    ImGuiContext *g                 = nullptr;
    int          surfaceWidth       = 0;
    int          surfaceHigh        = 0;
   bool ActivityState= true;//Activity状态

    EGL();

    //Surface生命周期
    void onSurfaceCreate(JNIEnv *env, jobject surface, int SurfaceWidth, int SurfaceHigh);

    bool isChage = false;

    void onSurfaceChange(int surfaceWidth, int SurfaceHigh);

    bool isDestroy = false;

    void onSurfaceDestroy();
    void ShowStyleEditor(ImGuiStyle* ref);

    void setSaveSettingsdir(string &dir);

    void setinput(ImguiAndroidInput *input_);

    ~EGL() {
    }

};
#endif