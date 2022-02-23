package com.vrkcreations.lockerauthenticator;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.biometric.BiometricPrompt;
import androidx.core.content.ContextCompat;


import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
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

    Gson gson = new Gson();

    Fingerprint_activity_data fingerprintActivityData = null;
    Button show_password_button;
    Button authenticate_button;
    Button control_button;
    TextView secondary_password_indicator_textview;
    TextView primary_password_indicator_textview;
    TextView auth_type_textview;
    EditText secondary_password_edittext;
    EditText primary_password_edittext;




    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fingerprint_password);
        findViewById(R.id.activity_fingerprint_password).setVisibility(View.INVISIBLE);

        init_views();

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
        primary_password_edittext.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if(charSequence.length()<8){
                    primary_password_edittext.setError("Minimum password length is 8");
                    authenticate_button.setVisibility(View.INVISIBLE);
                }else {
                    primary_password_edittext.setError(null);
                    if(auth_type_textview.getText().toString().equals("LOGIN")){
                        authenticate_button.setVisibility(View.VISIBLE);
                        hideKeyboard(fingerprint_password.this);
                    }

                }
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        secondary_password_edittext.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if(primary_password_edittext.getError()!=null){
                    secondary_password_edittext.setError("rule violation in password field");
                    authenticate_button.setVisibility(View.INVISIBLE);
                }else if(!primary_password_edittext.getText().toString().equals(secondary_password_edittext.getText().toString())){
                    secondary_password_edittext.setError("Re-entered password should match original one");
                    authenticate_button.setVisibility(View.INVISIBLE);
                }else{
                    secondary_password_edittext.setError(null);
                    authenticate_button.setVisibility(View.VISIBLE);
                    hideKeyboard(fingerprint_password.this);
                }
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

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
            if (fingerprintActivityData.getPassword_set()) {
                setviewvisiblity("login");
            }else{
                setviewvisiblity("register");
            }
        }

    }

    private void init_views(){
        authenticate_button=findViewById(R.id.authenticate_button);
        show_password_button=findViewById(R.id.show_password_button);
        control_button=findViewById(R.id.control_button);
        secondary_password_indicator_textview=findViewById(R.id.secondary_password_indicator_textview);
        primary_password_indicator_textview=findViewById(R.id.primary_password_indicator_textview);
        auth_type_textview=findViewById(R.id.password_mode_indicator_textview);
        secondary_password_edittext=findViewById(R.id.secondary_password_edittext);
        primary_password_edittext=findViewById(R.id.primary_password_edittext);
    }

    private void setviewvisiblity(String type){

        if(type.equals("login")){
            auth_type_textview.setText("LOGIN");
            secondary_password_edittext.setVisibility(View.GONE);
            secondary_password_indicator_textview.setVisibility(View.GONE);
            authenticate_button.setText("LOGIN");
            show_password_button.setText("SHOW PASSWORD");
        }else if(type.equals("register")){
            auth_type_textview.setText("REGISTER");
            secondary_password_edittext.setVisibility(View.VISIBLE);
            secondary_password_indicator_textview.setVisibility(View.VISIBLE);
            authenticate_button.setText("REGISTER");
            show_password_button.setText("SHOW PASSWORD");
        }


    }

    public void change_show_password(View v){
        if(show_password_button.getText().equals("HIDE PASSWORD")){
            primary_password_edittext.setTransformationMethod(PasswordTransformationMethod.getInstance());
            secondary_password_edittext.setTransformationMethod(PasswordTransformationMethod.getInstance());
            show_password_button.setText("SHOW PASSWORD");
        }else{
            primary_password_edittext.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
            secondary_password_edittext.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
            show_password_button.setText("HIDE PASSWORD");
        }

    }

    public void toast(String message, int length) {
        Toast.makeText(getApplicationContext(), message, length).show();
    }

    public static void hideKeyboard(Activity activity) {
        InputMethodManager imm = (InputMethodManager) activity.getSystemService(Activity.INPUT_METHOD_SERVICE);
        //Find the currently focused view, so we can grab the correct window token from it.
        View view = activity.getCurrentFocus();
        //If no view currently has focus, create a new one, just so we can grab a window token from it
        if (view == null) {
            view = new View(activity);
        }
        imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }

}