//
// Created by admin on 2022/6/10.
//

#include "EGL.h"
//#include "KingImage.h"
//#include "ImGuiDraw.h"
EGL::EGL() {
    mEglDisplay = EGL_NO_DISPLAY;
    mEglSurface = EGL_NO_SURFACE;
    mEglConfig  = nullptr;
    mEglContext = EGL_NO_CONTEXT;
   // FPS         = 90;
}

static bool RunInitImgui;//检测imgui是否初始化过


int EGL::initEgl() {
    //1、
    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mEglDisplay == EGL_NO_DISPLAY) {
        LOGE("eglGetDisplay error=%u", glGetError());
        return -1;
    }
    LOGE("生成mEglDisplay");
    //2、
    EGLint *version = new EGLint[2];
    if (!eglInitialize(mEglDisplay, &version[0], &version[1])) {
        LOGE("eglInitialize error=%u", glGetError());
        return -1;
    }
    LOGE("eglInitialize成功");
    //3、
    const EGLint attribs[] = {EGL_BUFFER_SIZE, 32, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8,
                              EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_DEPTH_SIZE, 8, EGL_STENCIL_SIZE, 8, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE};

    EGLint num_config;
    if (!eglGetConfigs(mEglDisplay, NULL, 1, &num_config)) {
        LOGE("eglGetConfigs  error =%u", glGetError());
        return -1;
    }
    LOGE("num_config=%d", num_config);
    // 4、
    if (!eglChooseConfig(mEglDisplay, attribs, &mEglConfig, 1, &num_config)) {
        LOGE("eglChooseConfig  error=%u", glGetError());
        return -1;
    }
    LOGE("eglChooseConfig成功");
    //5、
    int attrib_list[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    mEglContext = eglCreateContext(mEglDisplay, mEglConfig, EGL_NO_CONTEXT, attrib_list);
    if (mEglContext == EGL_NO_CONTEXT) {
        LOGE("eglCreateContext  error = %u", glGetError());
        return -1;
    }
    // 6、
    mEglSurface = eglCreateWindowSurface(mEglDisplay, mEglConfig, SurfaceWin, NULL);
    if (mEglSurface == EGL_NO_SURFACE) {
        LOGE("eglCreateWindowSurface  error = %u", glGetError());
        return -1;
    }
    LOGE("eglCreateWindowSurface成功");
    //7、
    if (!eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
        LOGE("eglMakeCurrent  error = %u", glGetError());
        return -1;
    }
    LOGE("eglMakeCurrent成功");
    return 1;
}



int EGL::initImgui() {

    if (RunInitImgui){
        //如果初始化过，就只执行这段
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplAndroid_Init(this->SurfaceWin);
        ImGui_ImplOpenGL3_Init("#version 300 es");
        return 1;
    }
    RunInitImgui= true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    LOGE("CreateContext成功");
    io = &ImGui::GetIO();
    io->IniSavingRate = 10.0f;
    string SaveFile = this->SaveDir;
    SaveFile += "/save.ini";
   // LOGE("保存配置文件的位置:%s",SaveFile.c_str());
    io->IniFilename = SaveFile.c_str();




   // io->ConfigWindowsMoveFromTitleBarOnly = true;
    //ImGui::LoadIniSettingsFromDisk(SaveFile.c_str());
    ImGui_ImplAndroid_Init(this->SurfaceWin);
    LOGE(" ImGui_ImplAndroid_Init成功");
   // ImGui_ImplOpenGL3_Init("#version 100");
    ImGui_ImplOpenGL3_Init("#version 300 es");
    LOGE(" ImGui_ImplOpenGL3_Init成功");
    ImFontConfig font_cfg;
    font_cfg.FontDataOwnedByAtlas = false;
    imFont = io->Fonts->AddFontFromMemoryTTF((void *) OPPOSans_H, OPPOSans_H_size, 32.0f, &font_cfg, io->Fonts->GetGlyphRangesChineseFull());
    io->MouseDoubleClickTime = 0.0001f;
    LOGE(" Font成功");
    //UI窗体风格
    g = ImGui::GetCurrentContext();

        style =&ImGui::GetStyle();
        style->ScaleAllSizes(4.0f);//缩放尺寸
        style->FramePadding=ImVec2(10.0f,20.0f);

    //style->FramePadding=ImVec2(30,30);
   // style->ScaleAllSizes(1);//缩放尺寸

   // ImGui::SetWindowFontScale(150.0f);
//    imguiStyleLoad("myStyle");


   //自动读取主题
    string LoadFile = this->SaveDir;
    LoadFile += "/Style.dat";
    ImGuiStyle s;
    if (MyFile::ReadFile(&s,LoadFile.c_str())==1){
       *style=s;
        LOGE("读取主题成功");
    }


    return 1;
}


void EGL::onSurfaceCreate(JNIEnv *env, jobject surface, int SurfaceWidth, int SurfaceHigh) {
    LOGE("onSurfaceCreate");
    this->SurfaceWin       = ANativeWindow_fromSurface(env, surface);
    this->surfaceWidth     = SurfaceWidth;
    this->surfaceHigh      = SurfaceHigh;
    this->surfaceWidthHalf = this->surfaceWidth / 2;
    this->surfaceHighHalf  = this->surfaceHigh / 2;
    SurfaceThread = new std::thread([this] { EglThread(); });
    SurfaceThread->detach();
    LOGE("onSurfaceCreate_end");
}

void EGL::onSurfaceChange(int SurfaceWidth, int SurfaceHigh) {
    this->surfaceWidth     = SurfaceWidth;
    this->surfaceHigh      = SurfaceHigh;
    this->surfaceWidthHalf = this->surfaceWidth / 2;
    this->surfaceHighHalf  = this->surfaceHigh / 2;
    this->isChage          = true;
    LOGE("onSurfaceChange");
}

void EGL::onSurfaceDestroy() {

    LOGE("onSurfaceDestroy");
    this->isDestroy = true;

    std::unique_lock<std::mutex> ulo(Threadlk);
    //LOGE("onSurfaceDestroy2");
    cond.wait(ulo, [this] { return !this->ThreadIo; });
    delete SurfaceThread;
    SurfaceThread = nullptr;

}
char ca[1024];
void EGL::EglThread() {
    LOGE("imguidgsbdf成");

    //LOGE("EglThreadstrat");
    this->initEgl();
   // LOGE("Egl初始化完成");
    this->initImgui();
//    LOGE("imgui初始化完成");
    ThreadIo = true;
   // AutoFPS.setAffinity();
//    getRandomColor(HealthColor, 200, 0.7);
    input->initImguiIo(io);
    input->setImguiContext(g);
    input->setwin(this->g_window);
   // LOGE("surfaceWidth=%d,surfaceHigh=%d", this->surfaceWidth, this->surfaceHigh);

    while (true) {



        if (this->isChage) {
            glViewport(0, 0, this->surfaceWidth, this->surfaceHigh);
            this->isChage = false;
        }
        if (this->isDestroy) {
           // LOGE("isDes1");
            this->isDestroy = false;
           // LOGE("isDes2");
            ThreadIo = false;
            //LOGE("isDes3");
            cond.notify_all();
           // LOGE("isDes1");
            return;
        }


        this->clearBuffers();

        //如果Activity不处于活动状态就停止绘制
        if (!ActivityState)continue;//感觉没屌用
        imguiMainWinStart();

        //菜单
       // MainView();
        ImGui::SetNextWindowBgAlpha(0.7);
        //input->ItemActive  = ImGui::IsAnyItemActive();
        //input->ItemHovered = ImGui::IsAnyItemHovered();
        //input->ItemFocused = ImGui::IsAnyItemFocused();
        style->WindowTitleAlign = ImVec2(0.5, 0.5);//标题居中
        //style->WindowMinSize = ImVec2(900, 800);
        // FrameHeight = ImGui::GetFrameHeight();
        if (input->Scrollio && !input->Activeio) {
            input->funScroll(g->WheelingWindow ? g->WheelingWindow : g->HoveredWindow);
        }


       ImGui::Begin("menuTest");
        input->g_window = g_window = ImGui::GetCurrentWindow();
        ImGui::SetWindowSize({900, 800}, ImGuiCond_FirstUseEver);
        ImGui::SetWindowPos({0, 200}, ImGuiCond_FirstUseEver);
        ImGui::Text("hhhhhhhhhhhhhhh");
        ImGui::InputTextWithHint("##测试", "test", ca, IM_ARRAYSIZE(ca),
                                 ImGuiInputTextFlags_CallbackAlways,
                                 ImguiAndroidInput::inputCallback);
       ImGui::Button("tttcs测试");
       if(ImGui::Button("toast测试")){
           input->toast("我是一个测试");
       }
       ImGui::End();

        imguiMainWinEnd();
        this->swapBuffers();
        //WinList=ImGui::GetCurrentContext()->Windows;
        input->fps = this->FPS;
        //占用高，加个延迟缓和一下
       // sleep(0.1);
       // usleep(25000);
    }
}
int EGL::swapBuffers() {
    //opengl当前buff传递至屏幕
    if (eglSwapBuffers(mEglDisplay, mEglSurface)) {
        return 1;
    }
    LOGE("eglSwapBuffers  error = %u", glGetError());
    return 0;
}
void EGL::clearBuffers() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void EGL::imguiMainWinStart() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();

}

void EGL::imguiMainWinEnd() {
    // Render the Dear ImGui frame
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EGL::setSaveSettingsdir(string &dir) {
    this->SaveDir = dir;
    LOGE("保存路径=%s", SaveDir.c_str());
}

void EGL::setinput(ImguiAndroidInput *input_) {
    this->input = input_;
}



