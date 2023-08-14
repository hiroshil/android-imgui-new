package com.game.ui;

import android.app.Activity;
import android.app.Application;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.game.ui.Utils.CrashHandler;
import com.game.ui.Utils.utils;
import com.game.ui.Views.MyLayout;
import com.game.ui.Views.mySurfaceView;

public class App extends Application implements Application.ActivityLifecycleCallbacks {
    private static App sApp;
    public static boolean IsRun = false;
    //private static Context context;

    public static Activity GameContext;//游戏Activity上下文
    // private String PackageName="SGameRealActivity";
    private String PackageName="MainActivity";//调试菜单用 写对应的游戏Activity,或者编译后改也行
    public static App GetApp(){
        return sApp;
    }

    private static mySurfaceView surfaceView;//显示菜单的view
    private static MyLayout myLayout;//容器view
    private static View gameview;//游戏原本的view


    @Override
    public void onCreate() {
        super.onCreate();
        sApp = this;
        //注册Activity监控
        this.registerActivityLifecycleCallbacks(this);
        //错误处理
        CrashHandler.init(this);
        //so加载 顺便把包路径传过去
        GLES3JNILib.setFilePath(getFilesDir().getAbsolutePath());
    }


    @Override
    public void onActivityCreated(@NonNull Activity activity, @Nullable Bundle savedInstanceState) {
        //当Activity第一次被运行时调用此方法，可用于加载布局视图，获取控件命名空间等一些初始化工作


    }

    @Override
    public void onActivityStarted(@NonNull Activity activity) {
        //表示Activity正在被启动，已经从不可见到可见状态（不是指用户可见，指Activity在后台运行，没有出现在前台），但还是无法与用户进行交互。
        if (activity.getClass().getSimpleName().equals(PackageName)){

        }

    }

    @Override
    public void onActivityResumed(@NonNull Activity activity) {
        //表示Activity已经变为可见状态了，并且出现在前台工作了，也就是指用户可见了
        //判断Activity名字 显示
        if (activity.getClass().getSimpleName().equals(PackageName)){
            GameContext=activity;
            //Toast.makeText(activity,"开始运行"+activity.getClass().getSimpleName(),Toast.LENGTH_LONG).show();
            if (surfaceView==null) {
                //surface初始化
                surfaceView = new mySurfaceView(GameContext);
                surfaceView.setZOrderOnTop(true);//置顶
                //surfaceView.setZOrderMediaOverlay(true);
                surfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);

                //容器初始化
                if (myLayout==null){
                    myLayout = new MyLayout(GameContext);
                    LinearLayout.LayoutParams relLayoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
                    myLayout.setLayoutParams(relLayoutParams);
                    gameview=utils.getContentView(activity);//获取游戏原来的view组
                    utils.getContentViewGrout(activity).removeAllViews();//清除原来activity的所有view
                    myLayout.addView(gameview);//然后在容器里面添加原来的view
                    myLayout.addView(surfaceView);//添加附加层view
                }else {
                    return;
                }




                activity.setContentView(myLayout);//把容器设置成activity的默认布局

            }
            IsRun=true;


        }
    }

    @Override
    public void onActivityPaused(@NonNull Activity activity) {
        //表示Activity正在暂停，但Activity依然可见，可以执行一些轻量级操作，但一般不会进行太多操作，因为这样会影响用户体验。
    }

    @Override
    public void onActivityStopped(@NonNull Activity activity) {
        //表示Activity已经暂停，
        IsRun=false;
    }

    @Override
    public void onActivitySaveInstanceState(@NonNull Activity activity, @NonNull Bundle outState) {

    }

    @Override
    public void onActivityDestroyed(@NonNull Activity activity) {
        //表示活动即将被销毁。
        //判断Activity名字 销毁
        if (activity.getClass().getSimpleName().equals(PackageName)){
            surfaceView=null;
            gameview=null;
            myLayout=null;
            GameContext=null;
        }
    }
}
