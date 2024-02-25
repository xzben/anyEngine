package com.example.anyengine;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLSurfaceRender implements GLSurfaceView.Renderer{

    public void onSurfaceCreated(GL10 gl, EGLConfig config){//视口初始化时
        AssetManager am = MainActivity.getInstance().getAssets();
        GhostLib.Init(am);
    }

    public void onSurfaceChanged(GL10 gl, int width, int height){//视口改变时
        GhostLib.OnViewportChanged(width, height);
    }

    public void onDrawFrame(GL10 gl){//绘制时候
        GhostLib.Render();
    }
}