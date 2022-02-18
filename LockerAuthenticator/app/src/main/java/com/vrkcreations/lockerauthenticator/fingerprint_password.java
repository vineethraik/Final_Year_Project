package com.vrkcreations.lockerauthenticator;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.biometric.BiometricPrompt;
import androidx.core.content.ContextCompat;


import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.google.gson.Gson;


import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Executor;

public class fingerprint_password extends AppCompatActivity {

    private final String data_key = "Fingerprint_activity_data";
    private final String shared_preferences_key = "Offline_data";

    private Executor executor_login;
    private BiometricPrompt biometricPrompt;
    private BiometricPrompt.PromptInfo promptInfo;


    SharedPreferences sharedPreferences;
    SharedPreferences.Editor offline_data;

    Fingerprint_activity_data fingerprintActivityData = null;

    Gson gson = new Gson();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fingerprint_password);
        findViewById(R.id.activity_fingerprint_password).setVisibility(View.INVISIBLE);


        init();

        fingerprint_init();

        listeners();

        biometricPrompt.authenticate(promptInfo);


    }

    private void fingerprint_init() {
        executor_login = ContextCompat.getMainExecutor(this);
        biometricPrompt = new BiometricPrompt(fingerprint_password.this, executor_login, new BiometricPrompt.AuthenticationCallback() {

            @Override
            public void onAuthenticationError(int errorCode, @NonNull CharSequence errString) {
                super.onAuthenticationError(errorCode, errString);
                Toast.makeText(getApplicationContext(), errorCode + ":" + errString, Toast.LENGTH_LONG).show();

            }

            @Override
            public void onAuthenticationSucceeded(@NonNull BiometricPrompt.AuthenticationResult result) {
                super.onAuthenticationSucceeded(result);
                Toast.makeText(getApplicationContext(), result.toString(), Toast.LENGTH_LONG).show();
                findViewById(R.id.activity_fingerprint_password).setVisibility(View.VISIBLE);

            }

            @Override
            public void onAuthenticationFailed() {
                super.onAuthenticationFailed();
                Toast.makeText(getApplicationContext(), "try again", Toast.LENGTH_LONG).show();
                Timer timer = new Timer();
                TimerTask timerTask = new TimerTask() {
                    @Override
                    public void run() {
                        runOnUiThread(() -> {
                            timer.cancel();
                            finish();
                        });

                    }
                };
                timer.schedule(timerTask, 2000);

            }
        });

        promptInfo = new BiometricPrompt.PromptInfo.Builder()
                .setTitle("FingerPrint Authentication")
                .setSubtitle("Provide a valid fingerPrint to access the app")
                .setNegativeButtonText("Cancel")
                .build();


    }

    private void listeners() {
    }

    private void init() {
        sharedPreferences = getSharedPreferences(shared_preferences_key, MODE_PRIVATE);
        offline_data = sharedPreferences.edit();

        if (!sharedPreferences.contains(data_key)) {
            offline_data.putString(data_key, gson.toJson(new Fingerprint_activity_data()));
            offline_data.commit();
            startActivity(new Intent(this, fingerprint_password.class));
        } else {
            fingerprintActivityData = gson.fromJson(sharedPreferences.getString(data_key, gson.toJson(new Fingerprint_activity_data())), Fingerprint_activity_data.class);
            if (!fingerprintActivityData.getPassword_set()) {

            }
        }

    }

    public void toast(String message, int length) {
        Toast.makeText(getApplicationContext(), message, length).show();
    }

}