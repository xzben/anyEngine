#include <jni.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <iostream>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#define HANGYU_LOG_TAG "HANGYUOpenGLES"

static  AAssetManager  *sAAssetManager = nullptr;//定义一个资源相关指针变量
extern "C" JNIEXPORT void JNICALL
Java_com_example_anyengine_GhostLib_Init(JNIEnv *env, jclass clazz, jobject am) {
    sAAssetManager = AAssetManager_fromJava(env,am);//初始化资源指针
    __android_log_print(ANDROID_LOG_INFO,HANGYU_LOG_TAG,"Init");
    glClearColor(1.f,0.4f,0.6f,1.0f);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_anyengine_GhostLib_OnViewportChanged(JNIEnv *env, jclass clazz, jint width,
                                                      jint height) {
    glViewport(0,0,width,height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_anyengine_GhostLib_Render(JNIEnv *env, jclass clazz) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}