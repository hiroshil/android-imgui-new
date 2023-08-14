package com.game.ui;

import android.app.Activity;
import android.graphics.PixelFormat;
import android.view.View;
import android.widget.LinearLayout;

import androidx.annotation.NonNull;

import com.game.ui.Utils.utils;
import com.game.ui.Views.MyLayout;
import com.game.ui.Views.mySurfaceView;

public class Launcher {
    public static Activity GameContext;//游戏Activity上下文


    private static mySurfaceView surfaceView;//显示菜单的view
    private static MyLayout myLayout;//容器view
    private static View gameview;//游戏原本的view

///另一种启动方法
    public static void Init(@NonNull Activity activity) {
        GLES3JNILib.setFilePath(activity.getFilesDir().getAbsolutePath());
        GameContext = activity;
        //Toast.makeText(activity,"开始运行"+activity.getClass().getSimpleName(),Toast.LENGTH_LONG).show();
        if (surfaceView == null) {
            //surface初始化
            surfaceView = new mySurfaceView(activity);
            surfaceView.setZOrderOnTop(true);//置顶
            //surfaceView.setZOrderMediaOverlay(true);
            surfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);

            //容器初始化
            if (myLayout == null) {
                myLayout = new MyLayout(GameContext);
                LinearLayout.LayoutParams relLayoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
                myLayout.setLayoutParams(relLayoutParams);
                gameview = utils.getContentView(activity);//获取游戏原来的view组
                utils.getContentViewGrout(activity).removeAllViews();//清除原来activity的所有view
                myLayout.addView(gameview);//然后在容器里面添加原来的view
                myLayout.addView(surfaceView);//添加附加层view
                activity.setContentView(myLayout);//把容器设置成activity的默认布局
            } else {
                return;
            }



        }
    }
}
