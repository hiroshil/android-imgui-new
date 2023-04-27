package com.game.ui;

import android.view.InputEvent;
import android.view.Surface;

public class GLES3JNILib {

    static {
        System.loadLibrary("mui");
        //System.loadLibrary("imgui");
    }


    public static native void setFilePath(String ph);

    //public static native void setSDdir(String ph);

    public static native void surfaceDestroyed();

    public static native void surfaceChange(int width, int high);

    public static native void surfaceCreate(Surface surface, int width, int high);

    //public static native CallData[] GetImguiwinsize();
   // public static native float[] GetImguiwinsize(float x,float y);
    //public static native void MotionEventClick(int Event_getAction, float PosX, float PosY);
    public static native boolean MotionEventClick(int Event_getAction, float PosX, float PosY);

    public static native void inputcharOnJNI(String msg);

    public static native void sendKeyEvent_JNI(int action, int code);

    public static native void finishComposingText_JNI();

    //public static native void setmOrientation(int orientation);
   // public static native Boolean sendKeyEvent(InputEvent inputEvent);

    public static native int Select_all();

    public static native String Cut();

    public static native void Paste(String data);

    public static native String Copy();

    public static native void sendDelKey(int code);

    //public static native void ActivityState(boolean state);


   // public static native void StrTest(String s);

}
