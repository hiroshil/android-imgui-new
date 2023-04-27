package com.game.ui.Views;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.InputType;
import android.util.AttributeSet;
import android.util.Log;
import android.view.HapticFeedbackConstants;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.Toast;

import androidx.annotation.NonNull;

import com.game.ui.App;
import com.game.ui.GLES3JNILib;


public class mySurfaceView extends SurfaceView implements SurfaceHolder.Callback {


    //private Orientation orientation;
    //private TouchView mtouch;
    int action;
    private static WindowManager.LayoutParams mtouchParams;
    private static Toast toast;
    private static Bundle bundle;
    private boolean Surface=false;//Surfacesf 是否被销毁
    private static mySurfaceView surfaceView;
    private static InputMethodManager manager;
    private MyInputConnection inputConnection;
    private static float x=0,y=0;
    private static int ISWantCaptureMouse=0;

    public static native void init();

    public mySurfaceView(Context context) {

        this(context, null);
        surfaceView=this;
        Log.e("NDK","View创建");
        getHolder().addCallback(this);
        bundle=new Bundle();
        inputConnection=new MyInputConnection(surfaceView,true);
        manager = (InputMethodManager) App.GameContext.getSystemService(Context.INPUT_METHOD_SERVICE);
        Log.e("NDK","View创建完毕");
        init();
        //setFocusable(true);
        //setFocusableInTouchMode(true);
        //this.setOnTouchListener(new OnTouchListenerTest());
        //setClickable(true);//解决只有触摸ACTION_DOWN

    }



   /* public mySurfaceView(Context context) {

        this(context, null);
        Log.e("NDK","View创建");
        getHolder().addCallback(this);
        orientation = new Orientation(context);
        mtouch = new TouchView(context);
        mtouch.initView();
        Log.e("NDK","View创建完毕");
    }*/



    public mySurfaceView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
        getHolder().addCallback(this);
        //orientation = new Orientation(context);
       // mtouch = new TouchView(context);
       // mtouch.initView();
    }

    public mySurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        getHolder().addCallback(this);
        //orientation = new Orientation(context);
       // mtouch = new TouchView(context);
       // mtouch.initView();
    }



    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        //GLES3JNILib.ActivityState(true);
        GLES3JNILib.surfaceCreate(holder.getSurface(), this.getWidth(), this.getHeight());
        App.IsRun=true;
        Log.e("NDK-java", "surfaceCreated");
        Surface=true;
    }



    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {

       // Resources rs = getResources();
       // DisplayMetrics dm = rs.getDisplayMetrics();
        //int mScreenHeight2 = dm.heightPixels;
       // int mScreenWidth2 = dm.widthPixels;
        GLES3JNILib.surfaceChange(width, height);

        Log.e("NDK-java", "surfaceChanged");
    }



    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

       // GLES3JNILib.ActivityState(false);
        GLES3JNILib.surfaceDestroyed();
        Log.e("NDK-java", "surfaceDestroyed");
        Surface=false;
        App.IsRun=false;
    }

    @Override
    public boolean onCheckIsTextEditor() {
        return true;
    }

    //是否在窗口内
    public static void WantCaptureMouse(int is){
        ISWantCaptureMouse=is;
    }





    @Override
    public boolean onTouchEvent(MotionEvent event) {
       // surfaceView.setZOrderOnTop(true);
        boolean bl=false;


           // Log.e("NDK-java", "actionB=" + event.getAction());
            // bl= GLES3JNILib.sendKeyEvent(2,0,0,event.getAction(),event.getMetaState(),event.getRawX(),event.getRawY(),event.getButtonState() );
        if (Surface)
           bl= GLES3JNILib.MotionEventClick(event.getAction(), x = event.getRawX(), y = event.getRawY());


            return bl;
            //if (bl)return true;
       // return true;
        //Log.e("NDK-java", "事件1");
        //return super.onTouchEvent(event);
    }




    //方法，需要返回一个InputConnect对象，这个是和输入法输入内容的桥梁。
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        // outAttrs就是我们需要设置的输入法的各种类型最重要的就是:
        outAttrs.imeOptions = EditorInfo.IME_FLAG_NO_FULLSCREEN |EditorInfo.IME_ACTION_DONE;
        outAttrs.inputType = InputType.TYPE_TEXT_FLAG_AUTO_CORRECT | InputType.TYPE_TEXT_FLAG_AUTO_COMPLETE |InputType.TYPE_MASK_FLAGS ;
        return inputConnection;
    }

    @SuppressLint("HandlerLeak")
    public static Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            toast = Toast.makeText(App.GameContext, msg.getData().getString("JNI_msg") , Toast.LENGTH_SHORT);
            toast.show();
        };
    };

    public static void mShow(String msg){
        Message message = handler.obtainMessage();
        bundle.putString("JNI_msg",msg);
        message.setData(bundle);
        handler.sendMessage(message);
    }
    public static void mIO(String psmsg,int pos, int io){
        int[] ioi = new int[2];
        ioi[0] = pos;
        ioi[1] = io;
        if (io>100){
            ioi[1] -= 100;
            surfaceView.performHapticFeedback(HapticFeedbackConstants.KEYBOARD_PRESS,HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING
            );
        }
        Message message = handlerm.obtainMessage();

//        Log.e("NDK","ioset"+"pos="+pos+"io="+io);
        bundle.putIntArray(psmsg,ioi);
        message.setData(bundle);//通过setData将字符串发送
        handlerm.sendMessage(message);

    }
    @SuppressLint("HandlerLeak")
    public static Handler handlerm = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            int[] io = new int[2];
            if ((io = msg.getData().getIntArray("psio")) != null) {
                ioset(io);
            }
        }
    };
    public static void ioset(int[] io){
        switch (io[0]) {
            case 2: {
                if ( io[1] == 1 ){

                    try {
                        Thread.sleep(500);
                    } catch (InterruptedException exception) {
                        exception.printStackTrace();
                    }
                    openInput();
                }else {
                    closeInput();


                }
            }
            break;
            case 5: {
                if ( io[1] == 1 ){
                    surfaceView.performHapticFeedback(HapticFeedbackConstants.KEYBOARD_PRESS,HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING
                    );
                }else {
                    surfaceView.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS,HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
                }
            }
            break;
            case 3: {
                isLongTouch((int)x,(int)y);
            }
            break;
        }
    }

    static boolean Inputio = false;

    //如果输入法关，则开启输入法
    public static boolean openInput(){

        // return showSoftInput();
        if (!Inputio){

            Log.e("NDK", "唤醒输入法");
            //        获取 接受焦点的资格
            surfaceView.setFocusable(true);
            //        获取 焦点可以响应点触的资格
            surfaceView.setFocusableInTouchMode(true);
            //        请求焦点
            boolean ck = surfaceView.requestFocus();
            Log.e("NDK","requestFocus:"+ck);
            //        弹出键盘
            ck = manager.showSoftInput(surfaceView, InputMethodManager.SHOW_FORCED);
//            manager.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS);
            Log.e("NDK","showSoftInput:"+ck);
            if (manager.isActive()) {
                Inputio = true;
                Log.e("NDK", "唤醒输入法成功");
               surfaceView.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS,HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
                return Inputio;
            } else {
                GLES3JNILib.finishComposingText_JNI();
                Log.e("NDK", "唤醒输入法失败");
                return Inputio;
            }
        }
        return Inputio;
    }
    /**
     * 如果输入法开，则关闭输入法
     */
    public static boolean closeInput(){
        if (Inputio){
            Log.e("NDK", "关闭输入法");
            //        取消焦点
            surfaceView.setFocusable(false);
            //        取消 焦点可以响应点触的资格
            surfaceView.setFocusableInTouchMode(false);
            //       清除焦点
            surfaceView.clearFocus();
            //       隐藏悬浮窗
            manager.hideSoftInputFromWindow(surfaceView.getWindowToken(), 0);

            if (manager.isActive()) {
                Inputio = false;
                //setmyFocusable(false);
                surfaceView.performHapticFeedback(HapticFeedbackConstants.LONG_PRESS,HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
                Log.e("NDK", "关闭输入法成功");
            } else {
                Log.e("NDK", "关闭输入法失败");
            }
            return Inputio;
        }
        return Inputio;
    }

    public static void isLongTouch(int x,int y){
        surfaceView.performHapticFeedback(HapticFeedbackConstants.KEYBOARD_PRESS,HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
        //FloatService.Show_mFloatButton((int)x,(int)y);
        surfaceView.performHapticFeedback(HapticFeedbackConstants.KEYBOARD_PRESS,HapticFeedbackConstants.FLAG_IGNORE_GLOBAL_SETTING);
    }

    //显示输入法
    public static boolean showSoftInput() {

        InputMethodManager inputMethodManager = (InputMethodManager) App.GameContext.getSystemService(Context.INPUT_METHOD_SERVICE);
        return inputMethodManager.showSoftInput(surfaceView, 0);
    }
    //隐藏输入法
    public static boolean hideSoftInput() {
        InputMethodManager inputMethodManager = (InputMethodManager) App.GameContext.getSystemService(Context.INPUT_METHOD_SERVICE);
        if (inputMethodManager.isActive()) {
            Log.d("hickey", "hideSoftInput:" + "hideSoftInputFromWindow");
            return  inputMethodManager.hideSoftInputFromWindow(surfaceView.getWindowToken(), 0);
        }
        return false;
    }

    /**
     * 重写BaseInputConnection获取输入法按键KEY以及输入内容
     */
    private class MyInputConnection extends BaseInputConnection {
        //一般我们都是些一个BaseInputConnection的子类，而BaseInputConnection是实现InputConnection接口的。

        //需要注意的就是几个方法注意重写。
        public MyInputConnection(View targetView, boolean fullEditor) {
            super(targetView, fullEditor);
        }


        /**
         * @param text 获取按键输入内容，中文输入模式不可用
         * @param newCursorPosition 输入光标偏移，通常为1，如果为0无输入
         * @return
         */
        @Override
        public boolean setComposingText(CharSequence text, int newCursorPosition) {
            //note:获取到输入的字符
            Log.e("NDK", "setComposingText:" + text + "\t" + newCursorPosition);

            // postInvalidate();
            return true;
        }


        /**
         * @param text 选择输入法候选栏输入字符
         * @param newCursorPosition 通常为1
         * @return
         */
        @Override
        public boolean commitText(CharSequence text, int newCursorPosition) {
            //note:获取到输入的字符
            Log.e("NDK", "commitText:" + text + "\t" + newCursorPosition);
            GLES3JNILib.inputcharOnJNI(text.toString());

            //compo


            // postInvalidate();
            return true;
        }


        /**
         * @param event 获取按键KeyEvent
         * @return
         */
        @Override
        public boolean sendKeyEvent(KeyEvent event) {
            /** 当手指离开的按键的时候 */
            GLES3JNILib.sendKeyEvent_JNI(event.getAction(),event.getKeyCode());
            if (event.getAction()==KeyEvent.ACTION_DOWN){
                if (event.getKeyCode() == KeyEvent.KEYCODE_DEL) {
                    GLES3JNILib.sendDelKey(1);
                }
            }else if (event.getAction()==KeyEvent.ACTION_UP){
                if (event.getKeyCode() == KeyEvent.KEYCODE_DEL) {
                    GLES3JNILib.sendDelKey(0);
                }
            }

            Log.e("NDK", "Key="+event.getKeyCode());
            return true;
        }



        //当然删除的时候也会触发
        @Override
        public boolean deleteSurroundingText(int beforeLength, int afterLength) {
            Log.e("NDK", "deleteSurroundingText " + "beforeLength=" + beforeLength + " afterLength=" + afterLength);
            //处理有些输入法会只执行deleteSurroundingText 而不执行sendKeyEvent
            if (beforeLength == 1 || afterLength == 0 || beforeLength == 0) {
                // backspace
                return sendKeyEvent(new KeyEvent(KeyEvent.ACTION_DOWN ,KeyEvent.KEYCODE_DEL))
                        && sendKeyEvent(new KeyEvent(KeyEvent.ACTION_UP, KeyEvent.KEYCODE_DEL));
            }

            return true;
        }

        /**
         * @return 手动关闭输入法，结束输入
         */
        @Override
        public boolean finishComposingText() {
            //结束组合文本输入的时候，这个方法基本上会出现在切换输入法类型，点击回车（完成、搜索、发送、下一步）点击输入法右上角隐藏按钮会触发。
            Log.e("NDK", "finishComposingText");
            GLES3JNILib.finishComposingText_JNI();
            return true;
        }
    }


}
