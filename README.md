# AndroidImguiNew

#### 介绍
自己写的另一种imgui内部菜单实现方式，不需要悬浮窗权限，只显示在应用内部

#### 软件架构
无


#### 原理
安卓启动Activity之前会先启动Application
所以我们创建一个继承Application的类，并且在清单文件设置全局Application
在我们继承Application的类里面实现一个Application.ActivityLifecycleCallbacks接口
这个接口可以检测所有启动的Activity的生命周期,当检测到对应的游戏Activity的时候获取
Activity对应的布局view ,然后在对应的布局view上面addview一个显示imgui的SurfaceView
这样就能添加成功了.


#### 使用教程 
##  有两种方法  ##
#方法1//////
0.  如果你要嵌入游戏，需要以下几个步骤
1.  首先编译和和游戏同架构的so文件
2.  然后把so塞进游戏对应的lib文件夹里面
3.  修改清单文件AndroidManifest，把我们Application加上去 如果有的游戏自带Application的话，就让那个Application继承我们自己写的Application
4.  逆向后把我们编译的应用的classes.dex添加到游戏的解包目录里面,记得改名,比如classes2.dex

#方法2
前面0-2部分相同
3. 反编译后从MainActivity的onCreate里面复制Launcher的启动代码invoke-static {p0}, Lcom/game/ui/Launcher;->Init(Landroid/app/Activity;)V
4. 然后在游戏的Activity的onCreate里面添加上述代码记得在setContentView方法之后再添加,不然会崩溃,详情可以看视频
#### 使用说明

#### 演示图
<img src="https://gitee.com/alexmmc/android-imgui-new/raw/master/Images/Screenshot_2023-04-27-21-17-27-04.jpg" width="600" height="400"
alt="演示1随便找的游戏"/><br/>
<img src="https://gitee.com/alexmmc/android-imgui-new/raw/master/Images/Screenshot_2023-04-27-21-22-26-95.jpg" width="300" height="500"
alt="演示2"/><br/>