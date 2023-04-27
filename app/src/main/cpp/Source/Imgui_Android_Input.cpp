//
// Created by Administrator on 2022/2/7.
//

#include "Imgui_Android_Input.h"

using namespace std;

void ImguiAndroidInput::Copy(ImGuiInputTextCallbackData *CallbackData) {
    Copybuf = (char *) malloc(CallbackData->SelectionEnd - CallbackData->SelectionStart + 2);
    bzero(Copybuf, CallbackData->SelectionEnd - CallbackData->SelectionStart + 2);
    strlcat(Copybuf, &CallbackData->Buf[CallbackData->SelectionStart], CallbackData->SelectionEnd - CallbackData->SelectionStart + 1);
}

string ImguiAndroidInput::JNI_Copy() {
    if (!io->WantTextInput) {
        return "";
    }
    std::unique_lock<std::mutex> ulo(Copylk);
    ImguiAndroidInput::ActiveInputsw = 0;
    cond.wait(ulo, [this] { return ImguiAndroidInput::ActiveInputsw == 10; });
    string tmp = Copybuf;
    if (Copybuf != nullptr) {
        free(Copybuf);
        Copybuf = nullptr;
    }
    return tmp;
}

void ImguiAndroidInput::Paste(ImGuiInputTextCallbackData *CallbackData) {
    if (CallbackData->HasSelection()) {
        CallbackData->DeleteChars(CallbackData->SelectionStart, CallbackData->SelectionEnd - CallbackData->SelectionStart);
    }
    CallbackData->InsertChars(CallbackData->CursorPos, Pastebuf);
}

void ImguiAndroidInput::JNI_Paste(string data) {
    if (!io->WantTextInput) {
        return;
    }
    std::unique_lock<std::mutex> ulo(Pastelk);
    Pastebuf = data.c_str();
    this->ActiveInputsw = 1;
    cond.wait(ulo, [this] { return this->ActiveInputsw == 11; });
}

void ImguiAndroidInput::SelectAll(ImGuiInputTextCallbackData *CallbackData) {
    CallbackData->SelectAll();
    SelectSize = CallbackData->SelectionEnd - CallbackData->SelectionStart;
}

int ImguiAndroidInput::JNI_SelectAll() {
    ImguiAndroidInput::SelectSize = 0;
    if (!io->WantTextInput) {
        return SelectSize;
    }
    std::unique_lock<std::mutex> ulo(Selectlk);
    ImguiAndroidInput::ActiveInputsw = 2;
    cond.wait(ulo, [this] { return this->ActiveInputsw == 12; });
    return this->SelectSize;
}

void ImguiAndroidInput::Cut(ImGuiInputTextCallbackData *CallbackData) {
    Copybuf = (char *) malloc(CallbackData->SelectionEnd - CallbackData->SelectionStart + 2);
    bzero(Copybuf, CallbackData->SelectionEnd - CallbackData->SelectionStart + 2);
    strlcat(Copybuf, &CallbackData->Buf[CallbackData->SelectionStart], CallbackData->SelectionEnd - CallbackData->SelectionStart + 1);
    CallbackData->DeleteChars(CallbackData->SelectionStart, CallbackData->SelectionEnd - CallbackData->SelectionStart);
}

string ImguiAndroidInput::JNI_Cut() {
    if (!io->WantTextInput) {
        return "";
    }
    std::unique_lock<std::mutex> ulo(Cutlk);
    this->ActiveInputsw = 3;
    cond.wait(ulo, [this] { return this->ActiveInputsw == 13; });
    string tmp = Copybuf;
    if (Copybuf != nullptr) {
        free(Copybuf);
        Copybuf = nullptr;
    }
    return tmp;
}

int ImguiAndroidInput::inputCallback(ImGuiInputTextCallbackData *CallbackData) {
    switch (ActiveInputsw) {
        case 0: {
            Copy(CallbackData);
            ActiveInputsw = 10;
            cond.notify_all();
//			LOGE("inputCallback.Copy完成");
        }
            break;
        case 1: {
            Paste(CallbackData);
            ActiveInputsw = 11;
            cond.notify_all();
//			LOGE("inputCallback.Paste完成");
        }
            break;
        case 2: {
            SelectAll(CallbackData);
            ActiveInputsw = 12;
            cond.notify_all();
//			LOGE("inputCallback.SelectAll完成");
        }
            break;
        case 3: {
            Cut(CallbackData);
            ActiveInputsw = 13;
            cond.notify_all();
//			LOGE("inputCallback.Cut完成");
        }
            break;
        default: {

        }
            break;
    }
//	LOGE("CallbackData->EventChar=%hu",CallbackData->EventChar);
    return 0;
}

void ImguiAndroidInput::addUTF8(const char *in_data) {

    io->AddInputCharactersUTF8(in_data);
//	LOGE("输入=%s,io=%d",in_data,io->WantTextInput);
//	LOGE("输入了字符串=%d",io->InputQueueCharacters.Size);
}

void ImguiAndroidInput::initImguiIo(ImGuiIO *io) {
    this->io = io;
    moveio   = false;
    f        = 1.0f;
    Copybuf  = nullptr;
    Pastebuf = nullptr;
    Inputio  = false;
    fps      = 60;
    if (max_fps == 0) {
        max_fps = 60;
    }
}

void ImguiAndroidInput::InputKey(int action, int code) {
    switch (action) {
        case InputAction::Action_DOWN: {
            if (code == 59) {
                io->KeyShift = true;
            }
            //按下回车关闭输入法
            if (code == 66) {
                Inputio=closeInput();
            }
            //io->AddKeyEvent(code, true);
            //io->KeyMap[code]=true;
            //LOGE("按下键值:%d",code);
            io->KeysDown[code] = true;
        }
            break;
        case InputAction::Action_UP: {
            if (code == 59) {
                io->KeyShift = false;
            }
            //弹起回车关闭输入法
            if (code == 66) {
                //重复两次 规避输入法隐藏失败
             Inputio= closeInput();
            }
           // LOGE("弹起键值:%d",code);
           // io->AddKeyEvent(code, false);
            //io->KeyMap[code]= false;
            io->KeysDown[code] = false;
        }
            break;
    }
//	LOGE("action = %d  code = %d",action,code);
//调整响应速度
    usleep(20000);
}


void ImguiAndroidInput::toast(string str) const {
    if (gClassInfo.jvm == nullptr || gClassInfo.pJclass == nullptr || gClassInfo.show == nullptr) {
        return;
    }
    JNIEnv  *env         = nullptr;
    bool    shouldDetach = false;
    int     result       = gClassInfo.jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED) {
        jint attachResult = gClassInfo.jvm->AttachCurrentThread(&env, NULL);
        if (attachResult >= 0) {
            shouldDetach = true;
        }
    }
    jstring tmp          = env->NewStringUTF(str.c_str());
    env->CallStaticVoidMethod(gClassInfo.pJclass, gClassInfo.show, tmp);
    env->DeleteLocalRef(tmp);
    if (shouldDetach) {
        gClassInfo.jvm->DetachCurrentThread();
    }
}
//打开输入法
bool ImguiAndroidInput::openInput(){
    if (gClassInfo.jvm == nullptr || gClassInfo.pJclass == nullptr || gClassInfo.openInput == nullptr) {
        return false;
    }
    JNIEnv  *env         = nullptr;
    bool    shouldDetach = false;
    int     result       = gClassInfo.jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED) {
        jint attachResult = gClassInfo.jvm->AttachCurrentThread(&env, NULL);
        if (attachResult >= 0) {
            shouldDetach = true;
        }
    }
    LOGE("触发输入法--窗口id%d",g_window->ID);
    jboolean tmp=false;

        tmp =  env->CallStaticBooleanMethod(gClassInfo.pJclass,gClassInfo.openInput);

    if(env->ExceptionCheck()){

        env->ExceptionDescribe();
        env->ExceptionClear();
        string s="检测到异常";
        toast(s);
        return false;

    }
    //LOGE("调用");
    if (shouldDetach) {
        gClassInfo.jvm->DetachCurrentThread();
    }
    //LOGE("调用1");
    return tmp == JNI_TRUE ? true : false;
}
//关闭输入法
bool ImguiAndroidInput::closeInput(){
    if (gClassInfo.jvm == nullptr || gClassInfo.pJclass == nullptr || gClassInfo.closeInput == nullptr) {
        return false;
    }
    JNIEnv  *env         = nullptr;
    bool    shouldDetach = false;
    int     result       = gClassInfo.jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED) {
        jint attachResult = gClassInfo.jvm->AttachCurrentThread(&env, NULL);
        if (attachResult >= 0) {
            shouldDetach = true;
        }
    }
    LOGE("隐藏输入法");
    jboolean tmp =  env->CallStaticBooleanMethod(gClassInfo.pJclass,gClassInfo.closeInput);
    if (shouldDetach) {
        gClassInfo.jvm->DetachCurrentThread();
    }
    return tmp == JNI_TRUE ? true : false;
}

void ImguiAndroidInput::ioset(jint pos, jint v) const {
    if (gClassInfo.jvm == nullptr || gClassInfo.pJclass == nullptr || gClassInfo.io == nullptr) {
        return;
    }
//	LOGE("开始ioset");
    JNIEnv  *env         = nullptr;
    bool    shouldDetach = false;
    int     result       = gClassInfo.jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED) {
        jint attachResult = gClassInfo.jvm->AttachCurrentThread(&env, NULL);
        if (attachResult < 0) {
//			LOGE("无法attach.JVM线程");
            return;
        }
        shouldDetach = true;
    }
    jstring tmp          = env->NewStringUTF("psio");
//	LOGE("set.pos=%d,v=%d", pos, v);
    env->CallStaticVoidMethod(gClassInfo.pJclass, gClassInfo.io, tmp, pos, v);
//	env->CallVoidMethod(gClassInfo.obj,gClassInfo.v,tmp,pos,v);
//	LOGE("准备释放jstring");
    env->DeleteLocalRef(tmp);
    if (shouldDetach) {
//		LOGE("准备释放jvm线程");
        gClassInfo.jvm->DetachCurrentThread();
    }
}

void ImguiAndroidInput::isLongTouch(int x, int y) {
    if (gClassInfo.jvm == nullptr || gClassInfo.pJclass == nullptr || gClassInfo.io == nullptr) {
        return;
    }
    JNIEnv  *env         = nullptr;
    bool    shouldDetach = false;
    int     result       = gClassInfo.jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (result == JNI_EDETACHED) {
        jint attachResult = gClassInfo.jvm->AttachCurrentThread(&env, NULL);
        if (attachResult < 0) {
//			LOGE("无法attach.JVM线程");
            return;
        }
        shouldDetach = true;
    }
    env->CallStaticVoidMethod(gClassInfo.pJclass,gClassInfo.isLongTouch,x,y);
    if (shouldDetach) {
//		LOGE("准备释放jvm线程");
        gClassInfo.jvm->DetachCurrentThread();
    }
}


//初始化
void ImguiAndroidInput::funMshowinit(jclass thiz, JNIEnv *env) {

    if (gClassInfo.show == nullptr) {
        (*env).GetJavaVM(&gClassInfo.jvm);
       // gClassInfo.pJclass = env->FindClass("com/example/imguimenu/TouchView");
		gClassInfo.pJclass = thiz;
        gClassInfo.show    = env->GetStaticMethodID(gClassInfo.pJclass, "mShow", "(Ljava/lang/String;)V");
        if (nullptr == gClassInfo.show) {
//			LOGE("can't find method mShow from JniClass");
            return;
        } else {
//			LOGE("find method mShow from JniClass");
        }
        gClassInfo.io = env->GetStaticMethodID(gClassInfo.pJclass, "mIO", "(Ljava/lang/String;II)V");
        if (nullptr == gClassInfo.io) {
//			LOGE("can't find method mIO from JniClass");
            return;
        } else {
//			LOGE("find method mIO from JniClass");
        }
        gClassInfo.openInput = env->GetStaticMethodID(gClassInfo.pJclass,"openInput","()Z");
        if (nullptr == gClassInfo.openInput) {
//			LOGE("can't find method mIO from JniClass");
            return;
        } else {
//			LOGE("find method mIO from JniClass");
        }
        gClassInfo.closeInput = env->GetStaticMethodID(gClassInfo.pJclass,"closeInput","()Z");
        if (nullptr == gClassInfo.closeInput) {
//			LOGE("can't find method mIO from JniClass");
            return;
        } else {
//			LOGE("find method mIO from JniClass");
        }
        gClassInfo.isLongTouch = env->GetStaticMethodID(gClassInfo.pJclass,"isLongTouch","(II)V");
        if (nullptr == gClassInfo.isLongTouch) {
//			LOGE("can't find method mIO from JniClass");
            return;
        } else {
//			LOGE("find method mIO from JniClass");
        }



        gClassInfo.obj = (*env).NewGlobalRef(thiz);
        if (nullptr == gClassInfo.obj) {
//			LOGE("can't find jobject");
            return;
        } else {
//			LOGE("find jobject");
        }
    }
}

void ImguiAndroidInput::longTouchLoop(){
    if (loopRun){
        return;
    }
    thread loop([this]{
        for (;;){
            if ( TOUCH_TIMER.islooptimestart ){
                if (TOUCH_TIMER.getlooptime()>150000000&&!isMouseMove&&io->WantTextInput){
                    LOGE("长按了编辑框");
                    ioset(3,0);
//                    isLongTouch((int)DOWN_x,(int)DOWN_y);
                    loopRun = false;
                    return ;
                }
                if (isMouseMove || !io->WantTextInput || !io->MouseDown[0] || !loopRun){
                    loopRun = false;
                    return;
                }
            }
            usleep(1000);
        }
    });
    loop.detach();
}



bool ImguiAndroidInput::InputTouchEvent(int event_get_action, float x, float y) {
//	LOGE("event.key=%d",event_get_action);
//bool ret= false;

   // LOGE("触发状态:%s",io->WantCaptureMouseUnlessPopupClose? "触发":"没触发");
   // LOGE("触发状态1:%s",g_window->HasCloseButton? "触发":"没触发");

    if (io->WantTextInput) {

//		LOGE("imgui输入获得焦点");
        if (!Inputio && event_get_action != eTouchEvent::TOUCH_OUTSIDE) {
//            ioset(2, 1);
            Inputio = openInput();
            //Inputio = openInput();
//            Inputio = io->WantTextInput;
//			LOGE("imgui呼出输入法%d",Inputio);
        }
    } else {
//		LOGE("imgui输入丢失焦点");
        if (Inputio) {
//            ioset(2, 0);
            Inputio = closeInput();
           // Inputio = closeInput();
//			LOGE("imgui关闭输入法%d",Inputio);
        }
    }
    if (ItemHovered) {
        Activeio = ItemHovered;
    }

    switch (event_get_action) {
        case eTouchEvent::TOUCH_OUTSIDE:
           // io->MouseDown[0] = false;
            io->AddMouseButtonEvent(0,0);
            moveio   = false;
            Activeio = false;
            Scrollio = false;
            loopRun = false;
            DOWN_x   = DOWN_y   = 0;
            if (Inputio) {
                Inputio = closeInput();
//			LOGE("imgui关闭输入法%d",Inputio);
            }
            break;
        case eTouchEvent::TOUCH_DOWN:
            DOWN_x = x;
            DOWN_y = y;
            io->AddMousePosEvent(x, y);
            io->AddMouseButtonEvent(0,1);
            Activeio   = ItemHovered;
            SetScrollX = 0.0f;
            SetScrollY = 0.0f;
            ScrollXMAX = 0.0f;
            ScrollYMAX = 0.0f;
            TOUCH_TIME = 0.0f;
            upio       = false;
           // Scrollio   = false;
            runScroll  = false;
            isMouseMove = false;
            TOUCH_TIMER.start();
            TOUCH_TIMER.looptimestart();
            longTouchLoop();
            loopRun = true;
            if (y <= g_window->TitleBarHeight() && !moveio) {
                moveio = true;
            }
            break;
        case eTouchEvent::TOUCH_UP:
            //io->MouseDown[0] = false;
            io->AddMouseButtonEvent(0,0);
            SetScrollX = x - DOWN_x;
            SetScrollY = y - DOWN_y;
            moveio     = false;
            Activeio   = false;
            runScroll  = false;
            loopRun = false;
            DOWN_x     = DOWN_y = 0;
            TOUCH_TIME = TOUCH_TIMER.stop(1);
            break;
        case eTouchEvent::TOUCH_MOVE:
            //io->MousePos.x = x;
           // io->MousePos.y                          = y;
            io->AddMousePosEvent(x, y);
            loopRun = false;
            //io->isMouseMove = true;
           /* if (y > g_window->TitleBarHeight() && abs(DOWN_x - x) > 3.0f  ||  abs(DOWN_y - y) > 3.0f ){
                io->isMouseMove = true;
               // Scrollio = true;
            }*/

//			LOGE("滑动");
            break;
        default:

            break;
    }

    if (io->WantCaptureMouse==0) {
        return false;
    } else {
        return true;
//		LOGE("imgui无焦点");
    }
}

float ImguiAndroidInput::funScroll() {
    nowScrollX = ImGui::GetScrollX();
    nowScrollY = ImGui::GetScrollY();
    runScroll  = true;
    if (io->MouseDown[0]) {
        if (nowScrollX > 0.0f || nowScrollX < ImGui::GetScrollMaxX()) {
            ImGui::SetScrollX(ImGui::GetScrollX() - io->MouseDelta.x);
        }
//        ImGui::SetScrollX(ImGui::GetScrollX() - io->MouseDelta.x);
//        ImGui::SetScrollY(ImGui::GetScrollY() - io->MouseDelta.y);
        if (nowScrollY > 0.0f || nowScrollY < ImGui::GetScrollMaxY()) {
            ImGui::SetScrollY(ImGui::GetScrollY() - io->MouseDelta.y);
        }
//		TOUCH_TIME = io->MouseDownDuration[0];
        upio = false;
    } else {
        if (!upio) {
            if (TOUCH_TIME > 300) {
                SetScrollX = 0;
                SetScrollY = 0;
                Scrollio   = false;
                runScroll  = false;
            }
            Seed_up    = abs(SetScrollX) / (TOUCH_TIME);
//			LOGE("SetScrollX=%f",SetScrollX);
            ScrollX    = SetScrollX / (TOUCH_TIME / 1000 * (float) fps);
            SetScrollX *= Seed_up * f;
            ScrollXMAX = SetScrollX;
//			LOGE("SetScrollX=%f,Seed_up=%f,time%f",SetScrollX,Seed_up,TOUCH_TIME);
            Seed_up    = abs(SetScrollY) / (TOUCH_TIME);
//			LOGE("SetScrollX=%f",SetScrollY);
            ScrollY    = SetScrollY / (TOUCH_TIME / 1000 * (float) fps);
            SetScrollY *= Seed_up * f;
            ScrollYMAX = SetScrollY;
//			LOGE("SetScrollY=%f,Seed_up=%f,time%f",SetScrollY,Seed_up,TOUCH_TIME);
            upio       = true;
        }
        ScrollXRatio = (SetScrollX / ScrollXMAX);
        ScrollYRatio = (SetScrollY / ScrollYMAX);
        if (nowScrollX > 0.0f && nowScrollX < ImGui::GetScrollMaxX()) {
            ImGui::SetScrollX(nowScrollX - ScrollX * ScrollXRatio);
        } else {
            ScrollX = 0.0f;
        }
        if (nowScrollY > 0.0f && nowScrollY < ImGui::GetScrollMaxY()) {
            ImGui::SetScrollY(nowScrollY - ScrollY * ScrollYRatio);
        } else {
            ScrollY = 0.0f;
        }
        SetScrollX -= (ScrollX * ScrollXRatio);
        SetScrollY -= (ScrollY * ScrollYRatio);
        if (ScrollX * ScrollXRatio >= -0.9f && ScrollX * ScrollXRatio <= 0.9f && ScrollY * ScrollYRatio >= -0.9f && ScrollY * ScrollYRatio <= 0.9f) {
            SetScrollX = 0;
            SetScrollY = 0;
            Scrollio   = false;
            runScroll  = false;
        }
//						LOGE("当前Window.ScrollXRatio=%f,.ScrollYRatio=%f,SetScroll.x%f,SetScroll.y%f,ScrollX=%f,ScrollY%f",ScrollXRatio,ScrollYRatio,SetScrollX,SetScrollY,ScrollX,ScrollY);
    }
    return nowScrollX;
}

void ImguiAndroidInput::setMaxFPS(int MAX_FPS) {
    this->max_fps = MAX_FPS;
//	LOGE("设置FPS=%d",MAX_FPS);
}


void ImguiAndroidInput::StartLockWheelingWindow(ImGuiWindow *window) {
    if (g->WheelingWindow == window) {
        return;
    }
    g->WheelingWindow            = window;
    g->WheelingWindowRefMousePos = g->IO.MousePos;
    //g->WheelingWindowTimer       = 2.0f;
}


float ImguiAndroidInput::funScroll(ImGuiWindow *Window) {
    if (!Window) {
        return 0.0f;
    }
    StartLockWheelingWindow(Window);
    nowScrollX = Window->Scroll.x;
    nowScrollY = Window->Scroll.y;
    runScroll  = true;
    if (isMouseMove) {
        ImGui::SetScrollX(Window, Window->Scroll.x - io->MouseDelta.x);
        ImGui::SetScrollY(Window, Window->Scroll.y - io->MouseDelta.y);
//		TOUCH_TIME = io->MouseDownDuration[0];
        upio = false;
    } else {
        if (!upio) {
            if (TOUCH_TIME > 300) {
                SetScrollX = 0;
                SetScrollY = 0;
                Scrollio   = false;
                runScroll  = false;
            }
            Seed_up    = abs(SetScrollX) / (TOUCH_TIME);
//			LOGE("SetScrollX=%f",SetScrollX);
            ScrollX    = SetScrollX / (TOUCH_TIME / 1000 * (float) fps);
            SetScrollX *= Seed_up * f;
            ScrollXMAX = SetScrollX;
//			LOGE("SetScrollX=%f,Seed_up=%f,time%f",SetScrollX,Seed_up,TOUCH_TIME);
            Seed_up    = abs(SetScrollY) / (TOUCH_TIME);
//			LOGE("SetScrollX=%f",SetScrollY);
            ScrollY    = SetScrollY / (TOUCH_TIME / 1000 * (float) fps);
            SetScrollY *= Seed_up * f;
            ScrollYMAX = SetScrollY;
//			LOGE("SetScrollY=%f,Seed_up=%f,time%f",SetScrollY,Seed_up,TOUCH_TIME);
            upio       = true;
        }
        ScrollXRatio = (SetScrollX / ScrollXMAX);
        ScrollYRatio = (SetScrollY / ScrollYMAX);

        if (nowScrollX > 0.0f && nowScrollX < Window->ScrollMax.x) {
            ImGui::SetScrollX(Window, nowScrollX - ScrollX * ScrollXRatio);
        } else {
            ScrollX = 0.0f;
        }
        if (nowScrollY > 0.0f && nowScrollY < Window->ScrollMax.y) {
            ImGui::SetScrollY(Window, nowScrollY - ScrollY * ScrollYRatio);
        } else {
            ScrollY = 0.0f;
        }
        SetScrollX -= (ScrollX * ScrollXRatio);
        SetScrollY -= (ScrollY * ScrollYRatio);
        if (ScrollX * ScrollXRatio >= -0.9f && ScrollX * ScrollXRatio <= 0.9f && ScrollY * ScrollYRatio >= -0.9f && ScrollY * ScrollYRatio <= 0.9f) {
            SetScrollX = 0;
            SetScrollY = 0;
            Scrollio   = false;
            runScroll  = false;
        }
//						LOGE("当前Window.ScrollXRatio=%f,.ScrollYRatio=%f,SetScroll.x%f,SetScroll.y%f,ScrollX=%f,ScrollY%f",ScrollXRatio,ScrollYRatio,SetScrollX,SetScrollY,ScrollX,ScrollY);
    }
    return nowScrollX;
}

void ImguiAndroidInput::setImguiContext(ImGuiContext *g_) {
    this->g = g_;
}

void ImguiAndroidInput::setwin(ImGuiWindow *g_window_) {
    this->g_window = g_window_;
}

