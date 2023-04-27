package com.game.ui.Views;

import android.content.Context;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;

import com.game.ui.App;


public class MyLayout extends FrameLayout {


    public MyLayout(Context context) {
        super(context);
    }


    //自己分发事件 解决bug
    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
       // Log.e("NDK-java", "actionAAA=" + ev.getAction());
       // return GLES3JNILib.sendKeyEvent(ev);
        //return true;
        if (!App.IsRun){
            return super.dispatchTouchEvent(ev);
        }


        boolean bl;
        if (getChildCount()>1){
            View v=getChildAt(getChildCount()-1);
            if(!v.dispatchTouchEvent(ev))
            {
                getChildAt(getChildCount()-2).dispatchTouchEvent(ev);
            }


        }else {
            getChildAt(0).dispatchTouchEvent(ev);
        }

        return true;
        //return super.dispatchTouchEvent(ev);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {

        if (!App.IsRun){
            return super.dispatchKeyEvent(event);
        }

        boolean bl;
        if (getChildCount()>1){
            View v=getChildAt(getChildCount()-1);
            if(!v.dispatchKeyEvent(event))
            {
                getChildAt(getChildCount()-2).dispatchKeyEvent(event);
            }


        }else {
            getChildAt(0).dispatchKeyEvent(event);
        }

       // return GLES3JNILib.sendKeyEvent(event);
        return true;
       // return super.dispatchKeyEvent(event);
    }

}
