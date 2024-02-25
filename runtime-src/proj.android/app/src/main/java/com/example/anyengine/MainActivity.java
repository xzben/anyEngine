package com.example.anyengine;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {
    static MainActivity mSelf;
    AppGLSurfaceView mGLview;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mSelf = this;
        mGLview = new AppGLSurfaceView(getApplication());//new一个视口
        setContentView(mGLview);
    }

    public static MainActivity getInstance(){
        return mSelf;
    }
}