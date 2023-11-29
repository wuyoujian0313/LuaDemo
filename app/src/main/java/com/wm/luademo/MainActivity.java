package com.wm.luademo;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;

import com.wm.lua.luaNativeDemo;
import com.wm.luademo.databinding.ActivityMainBinding;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;




public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;

    private String readStringFromAssets(String fileName) {
        //通过设备管理对象 获取Asset的资源路径
        AssetManager assetManager = this.getAssets();

        InputStream inputStream = null;
        InputStreamReader isr = null;
        BufferedReader br = null;

        StringBuffer sb =  new StringBuffer();
        try{
            inputStream = assetManager.open(fileName);
            isr = new InputStreamReader(inputStream);
            br = new BufferedReader(isr);

            sb.append(br.readLine());
            String line = null;
            while((line = br.readLine()) != null){
                sb.append("\n" + line);
            }

            br.close();
            isr.close();
            inputStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (br != null) {
                    br.close();
                }
                if (isr != null) {
                    isr.close();
                }
                if (inputStream != null) {
                    inputStream.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return sb.toString();
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        binding.luaButton.setOnClickListener(v -> {
            try {
                String luaScript = readStringFromAssets("main.lua");
                luaNativeDemo.JNIMain(v.getContext(),luaScript);
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        });
    }
}