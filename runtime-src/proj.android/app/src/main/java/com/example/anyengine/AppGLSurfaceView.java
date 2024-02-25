package com.example.anyengine;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class AppGLSurfaceView extends GLSurfaceView {
    android.opengl.GLSurfaceView.Renderer mRenderer;//渲染器
    public AppGLSurfaceView(Context context){
        super(context);
        setEGLContextClientVersion(2);//设置openGLES的版本号
        mRenderer = new GLSurfaceRender();//生成一个渲染器
        setRenderer(mRenderer);//设置渲染器
    }
}
