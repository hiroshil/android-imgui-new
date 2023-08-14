# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}
-keepattributes Signature #范型
##native方法不混淆
-keepclasseswithmembernames class * {
    native <methods>;
}
-keep class  com.game.ui.Views.mySurfaceView {
  public static boolean openInput();
  public static boolean closeInput();
  public static void mIO(java.lang.String,int,int);
  public static void mShow(java.lang.String);
  public static void isLongTouch(int ,int );
 }

#-keep class com.google.android.material.** {*;}
#-keep class androidx.** {*;}
#-keep public class * extends androidx.**
#-keep interface androidx.** {*;}
#-dontwarn com.google.android.material.**
#-dontnote com.google.android.material.**
#-dontwarn androidx.**

-keepclassmembernames class com.game.ui.Launcher{  *; }
# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile