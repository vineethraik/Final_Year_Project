package com.vrkcreations.lockerauthenticator;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;

public class splashscreen extends AppCompatActivity {

    Timer timer=new Timer();
    TimerTask timertask;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        timertask=new TimerTask() {
            @Override
            public void run() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        timer.cancel();

                        startActivity(new Intent(getApplicationContext(),fingerprint_password.class));
                    }
                });

            }
        };
        timer.schedule(timertask,2000);
    }


}