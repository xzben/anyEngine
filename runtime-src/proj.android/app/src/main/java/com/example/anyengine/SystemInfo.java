package com.example.anyengine;

import android.app.ActivityManager;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.telephony.TelephonyManager;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

public class SystemInfo {
    /**
     * 获取应用安装包路径(/data/app/包名.apk)
     * @return 应用安装包路径
     */
    public static String getAppPath() {
        return MainActivity.getInstance().getApplicationContext().getPackageResourcePath();
    }
    /**
     * 获取应用版本号
     *
     * @return 应用版本号
     */    /**
     * 获取应用版本号
     *
     * @return 应用版本号
     */
    public static String getAppVersion() {
        int versionCode = 0;
        PackageInfo info = getPackageInfo();
        if (null != info) {
            versionCode = info.versionCode;
        }
        return String.valueOf(versionCode);
    }
    /**
     * 获取data/data/包名/ 目录
     *
     * @return data/data/包名/
     */
    public static String getAppHomePath() {
        return MainActivity.getInstance().getApplicationInfo().dataDir + "/";
    }

    /**
     * 获取data/data/包名/files/ 目录
     *
     * @return data/data/包名/files/
     */
    public static String getInnerStoragePath() {
        return MainActivity.getInstance().getApplication().getFilesDir().getAbsolutePath() + "/";
    }

    private static Boolean isOuterValid = null;
    /**
     * 获取外部存储根目录
     *
     * @return SD卡根目录/包名/files
     */
    public static String getOuterStoragePath() {
        if (!hasOuterStorage()) {
            String path = getInnerStoragePath() + "__outer__/";
            try {
                new File(path).mkdirs();
            } catch (Exception e) {
                e.printStackTrace();
            }
            return path;
        }
        Context context = MainActivity.getInstance().getApplicationContext();
        String path = context.getExternalFilesDir(null) + "/";
        if (isOuterValid == null) {
            try {
                File file = new File(path, ".storage_test");
                File parent = file.getParentFile();
                if (!parent.exists()) {
                    parent.mkdirs();
                }
                if (!file.exists()) {
                    file.createNewFile();
                }
                FileOutputStream fos = new FileOutputStream(file);
                fos.write("ok".getBytes("UTF-8"));
                fos.flush();
                fos.close();
                isOuterValid = true;
            } catch (Exception e) {
                isOuterValid = false;
                Log.w("SystemInfo", "访问外部目录失败:" + e);
            }
        }
        if (!isOuterValid) {
            path = getInnerStoragePath() + "__outer__/";
            try {
                new File(path).mkdirs();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return path;
    }

    /**
     * 获取 SD 卡根目录
     *
     * @return SD卡根目录
     */
    public static String getSDcardRoot() {
        if (!hasOuterStorage()) {
            return getInnerStoragePath();
        }
        return Environment.getExternalStorageDirectory().getAbsolutePath() + "/";
    }

    /**
     * 判断SD是否可用
     *
     * @return true 可用；false 不可用
     */
    public static boolean hasOuterStorage() {
        String state = Environment.getExternalStorageState();
        return state != null && state.equals(Environment.MEDIA_MOUNTED);
    }

    /**
     * 获取uuid
     *
     * @return uuid
     */
    public static String getUUID() {
        SharedPreferences preference = PreferenceManager.getDefaultSharedPreferences(MainActivity.getInstance().getApplication());
        String identity = preference.getString("identity", null);
        if (identity == null) {
            identity = java.util.UUID.randomUUID().toString();
            preference.edit().putString("identity", identity);
        }
        return identity;
    }

    /**
     * 获取应用包名
     *
     * @return 包名
     */
    public static String getAppID() {
        return MainActivity.getInstance().getPackageName();
    }

    /**
     * 获取PackageInfo对象，如果获取不到返回null
     *
     * @return PackageInfo
     */
    public static PackageInfo getPackageInfo() {
        PackageInfo info = null;

        PackageManager manager = MainActivity.getInstance().getPackageManager();
        Context context = MainActivity.getInstance().getApplicationContext();
        try {
            info = manager.getPackageInfo(context.getPackageName(), 0);
        } catch (PackageManager.NameNotFoundException e) {

        }

        return info;
    }

    /**
     * 获取ApplicationInfo对象，如果获取不到返回null
     *
     * @return ApplicationInfo
     */
    public static ApplicationInfo getApplicationInfo() {
        ApplicationInfo appInfo = null;

        PackageManager manager = MainActivity.getInstance().getPackageManager();
        Context context = MainActivity.getInstance().getApplicationContext();
        try {
            appInfo = manager.getApplicationInfo(context.getPackageName(), 0);
        } catch (PackageManager.NameNotFoundException e) {

        }

        return appInfo;
    }

    /**
     * 获取应用版本号
     *
     * @return 返回应用版本号或者0;
     */
    public static int getVersionCode() {
        int versionCode = 0;
        PackageInfo info = getPackageInfo();
        if (null != info) {
            versionCode = info.versionCode;
        }
        return versionCode;
    }

    /**
     * 获取应用名
     *
     * @return 返回应用名或者"";
     */
    public static String getVersionName() {
        String versionName = "";
        PackageInfo info = getPackageInfo();
        if (null != info) {
            versionName = info.versionName;
        }
        return versionName;
    }

    /**
     * 获取data/data/包名/lib 目录
     *
     * @return data/data/包名/lib
     */
    public static String getLibraryPath() {
        return MainActivity.getInstance().getApplicationInfo().dataDir + "/lib";
    }

    public static String getLibraryNativePath() {
        String result = "";
        if (libraryPaths.size() == 0) {
            result = MainActivity.getInstance().getApplicationInfo().nativeLibraryDir;
        } else {
            for (String p : libraryPaths) {
                result = result + p + ";";
            }
            result = result + MainActivity.getInstance().getApplicationInfo().nativeLibraryDir;
        }
        System.out.println("SystemInfo.getLibraryNativePath: " + result);
        return result;
    }

    private static List<String> libraryPaths = new ArrayList<>();

    /**
     * 被tinker(quickupdate)调用。这个方法一定不能访问 AppActivity。 因为还没有初始化
     *
     * @param path
     */
    public static void addLibraryNativePath(String path) {
        System.out.println("SystemInfo.addLibraryNativePath: " + path);
        if (path == null) {
            return;
        }
        if (path.endsWith("/")) {
            path = path.substring(0, path.length() - 1);
        }
        if (!libraryPaths.contains(path)) {
            libraryPaths.add(0, path);
        }
    }

    /**
     * 获取当前系统语言
     *
     * @return 返回语言编码或者"";
     */
    public static String getLang() {
        return Locale.getDefault().getLanguage();
    }

    /**
     * 获取当前系统国家
     *
     * @return 返回国家编码或者"";
     */
    public static String getCountry() {
        return Locale.getDefault().getCountry();
    }

    /**
     * 获取设备id
     *
     * @return 返回设备id或者"";
     */
    public static String getDeviceId() {
        String deviceId = "";
        try {
            TelephonyManager telephonyManager = (TelephonyManager) MainActivity.getInstance()
                    .getApplication().getSystemService(
                            Context.TELEPHONY_SERVICE);
            if (null != telephonyManager) {
                deviceId = telephonyManager.getDeviceId();
            }
        } catch (SecurityException e) {
            e.printStackTrace();
        }
        return deviceId;
    }

    /**
     * 结束应用进程
     */
    public static void terminalProcess() {
        System.exit(0);
    }

    /**
     * 获取android引擎版本号
     *
     * @return android引擎版本号
     */
    public static String getNativeVersion() {
        return "6.0";
    }

    /**
     * 获取系统总内存
     *
     * @return android系统总内存
     */
    public static double getTotalMemory() {
        ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
        ActivityManager activityManager = (ActivityManager) MainActivity.getInstance()
                .getApplication().getSystemService(Context.ACTIVITY_SERVICE);
        activityManager.getMemoryInfo(mi);
        return (double) mi.totalMem;
    }
}
