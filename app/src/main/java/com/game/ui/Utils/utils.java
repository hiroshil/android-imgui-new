package com.game.ui.Utils;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Build;
import android.view.View;
import android.view.ViewGroup;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.UUID;

public class utils {

    /**
     * 获取Activity的第一个view
     * @param context 上下文
     * @return
     */
    public static View getContentView(Activity context)
    {
        return ((ViewGroup)context.findViewById(android.R.id.content)).getChildAt(0);
    }

    /**
     * 获取Activity的view组
     * @param context 上下文
     * @return
     */
    public static ViewGroup getContentViewGrout(Activity context)
    {
        return ((ViewGroup)context.findViewById(android.R.id.content));
    }



    /**
     * 获取相同设备的唯一ID。
     * @return
     */
    @SuppressLint("MissingPermission")
    public static String getUUID() {
        String serial = null;
        String m_szDevIDShort = "35" +
                Build.BOARD.length() % 10 + Build.BRAND.length() % 10 +
                Build.CPU_ABI.length() % 10 + Build.DEVICE.length() % 10 +
                Build.DISPLAY.length() % 10 + Build.HOST.length() % 10 +
                Build.ID.length() % 10 + Build.MANUFACTURER.length() % 10 +
                Build.MODEL.length() % 10 + Build.PRODUCT.length() % 10 +
                Build.TAGS.length() % 10 + Build.TYPE.length() % 10 +
                Build.USER.length() % 10; //13 位
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                serial = Build.getSerial();
            } else {
                serial = Build.SERIAL;
            }
            //API>=9 使用serial号
            return new UUID(m_szDevIDShort.hashCode(), serial.hashCode()).toString();
        } catch (Exception exception) {
            //serial需要一个初始化
            serial = "serial"; // 随便一个初始化
        }
        //使用硬件信息拼凑出来的15位号码
        return new UUID(m_szDevIDShort.hashCode(), serial.hashCode()).toString();
    }

    /**
     * 获取手机信息
     * 手机厂商  品牌  型号  手机版本
     * @return
     */
    public static String devices_info() {
        String devices_info= Build.MANUFACTURER + " " + Build.BRAND + " " + Build.MODEL + " Android" + Build.VERSION.RELEASE;
        return devices_info;
    }

    /**
     * 获取当前的时间
     * @return
     */
    public static String getTime() {
        Date date = new Date();// 创建一个时间对象，获取到当前的时间
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");// 设置时间显示格式
        String CurrentTime = sdf.format(date);
        return CurrentTime;
    }
}
