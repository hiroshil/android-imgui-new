# AndroidImguiNew

#### 介绍
自己写的另一种imgui内部菜单实现方式，不需要悬浮窗权限，只显示在应用内部

#### 软件架构
软件架构说明
####原理
安卓启动Activity之前会先启动Application
所以我们创建一个继承Application的类，并且在清单文件设置全局Application
在我们继承Application的类里面实现一个Application.ActivityLifecycleCallbacks接口
这个接口可以检测所有启动的Activity的生命周期,当检测到对应的游戏Activity的时候获取
Activity对应的布局view ,然后在对应的布局view上面addview一个显示imgui的SurfaceView
这样就能添加成功了.


#### 安装教程

1.  xxxx
2.  xxxx
3.  xxxx
0.  如果你要嵌入游戏，需要以下几个步骤
1.  首先编译和和游戏同架构的so文件
2.  然后把so塞进游戏对应的lib文件夹里面
3.  修改清单文件AndroidManifest，把我们Application加上去 如果有的游戏自带Application的话，就让那个Application继承我们自己写的Application
4.  逆向后把我们编译的应用的classes.dex添加到游戏的解包目录里面,记得改名,比如classes2.dex

#### 使用说明
