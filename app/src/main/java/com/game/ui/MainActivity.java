package com.game.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

import com.game.ui.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'ui' library on application startup.



    /*----------------------------------------------*/
    /*----------------------------------------------*/
    /*---------------请编译成release版本--------------*/
    /*---------------否则输入文本就会闪退---------------*/
    /*----------------------------------------------*/

    ///如果是显示在app的竖屏界面，记得自己给surface设置屏幕宽高，surfaceView自己测算的是屏幕宽高，
    // 触摸测算的是减去了状态栏标题栏的屏幕宽高，这会导致触摸偏移
    //当然，如果你是逆向嵌入横屏游戏,那就什么都不用管。

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Launcher.Init(MainActivity.this);

    }


}