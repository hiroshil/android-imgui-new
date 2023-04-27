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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


    }


}