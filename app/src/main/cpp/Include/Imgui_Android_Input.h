//
// Created by Administrator on 2022/2/7.
//

#ifndef FIND_ROODS_IMGUI_ANDROID_INPUT_H
#define FIND_ROODS_IMGUI_ANDROID_INPUT_H
#include <mutex>
#include "timer.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <jni.h>
#include "log.h"
#include <unistd.h>
using namespace std;
class ImguiAndroidInput{
	//java层io控制
	typedef struct ClassInfo {
		JavaVM *jvm;
		jobject obj;   //保存java对象
		jclass pJclass;   //保存java对象
		jclass  AssGLSL;
		jclass   GLFont;
		jmethodID show; //保存methodID
		jmethodID io; //保存methodID
        jmethodID openInput;
        jmethodID closeInput;
		jmethodID isLongTouch;
		jmethodID getmac;
	}ClassInfo;
	ClassInfo gClassInfo = {0};
	
	
	
	
	
	//触摸操作
	enum eTouchEvent {
		TOUCH_DOWN,
		TOUCH_UP,
		TOUCH_MOVE,
		TOUCH_CANCEL,
		TOUCH_OUTSIDE
	};

	bool isMouseMove;
	timer TOUCH_TIMER;
	float DOWN_x;
	float DOWN_y;
	bool moveio;
	float SetScrollX;
	float SetScrollY;
	float ScrollXMAX;
	float ScrollYMAX;
	float TOUCH_TIME;

	float nowScrollX;
	float nowScrollY;
	float Seed_up;

	float ScrollXRatio;
	float ScrollYRatio;
	
	//字符串输入输出内部函数
	enum InputAction {
		Action_DOWN,
		Action_UP
	};
	static inline int ActiveInputsw;
	static inline std::condition_variable cond;
	static inline char *Copybuf;
	std::mutex Copylk;
	static inline const char *Pastebuf;
	std::mutex Pastelk;
	static inline int SelectSize;
	std::mutex Selectlk;
	std::mutex Cutlk;
	ImGuiIO* io;
	ImGuiContext *g = nullptr;
	unsigned int touchTime = 0;
	static inline void Copy(ImGuiInputTextCallbackData *CallbackData);
	static inline void Paste(ImGuiInputTextCallbackData *CallbackData);
	static inline void SelectAll(ImGuiInputTextCallbackData *CallbackData);
	static inline void Cut(ImGuiInputTextCallbackData *CallbackData);
	void StartLockWheelingWindow(ImGuiWindow* window);
public:
	//初始化
	ImGuiWindow *g_window = nullptr;
	ImguiAndroidInput(){};
	void initImguiIo(ImGuiIO* io);
	void setwin(ImGuiWindow *g_window_);
	void setImguiContext(ImGuiContext *g_);
	void toast(string str) const;
	void ioset(jint pos,jint v) const;
	bool openInput();
	bool closeInput();
	void isLongTouch(int x,int y);
	bool loopRun = false;
	void longTouchLoop();
	void funMshowinit(jclass thiz, JNIEnv *env);
	void setMaxFPS(int MAX_FPS);
	
	//字符串输入输出公共函数
	static int inputCallback(ImGuiInputTextCallbackData *CallbackData);
	string JNI_Cut();
	int JNI_SelectAll();
	void JNI_Paste(string data);
	string JNI_Copy();
	void addUTF8(const char * in_data);
	void InputKey(int action,int code);
	
	bool Inputio;
	bool Scrollio;
	bool Activeio;
	float ScrollX;
	float ScrollY;
	float f;
	bool InputTouchEvent(int event_get_action,float x,float y);
	float funScroll();
    float funScroll(ImGuiWindow* Window);
	bool runScroll = false;
	bool upio = false;
	int fps;
	int max_fps = 0;
	//bool oldio   = false;
	//bool oldiow  = false;
	bool winio;
	bool fullwinio;
	float winWidth;
	float winHeight;
	float oldwinWidth;
	float oldwinHeight;
	bool ItemActive;
	bool ItemHovered;
	bool ItemFocused;
	bool ItemEdited;
	bool ItemScrollio;


};

#endif //FIND_ROODS_IMGUI_ANDROID_INPUT_H
