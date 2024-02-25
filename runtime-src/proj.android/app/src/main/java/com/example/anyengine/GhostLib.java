package com.example.anyengine;

import android.content.res.AssetManager;

public class GhostLib {
    static {
        System.loadLibrary("anyengine");
    }

    public static native void Init(AssetManager am);
    public static native void OnViewportChanged(int width, int height);
    public static native void Render();
}
