package com.vrkcreations.lockerauthenticator;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.biometric.BiometricPrompt;
import androidx.core.content.ContextCompat;


import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.telephony.SmsManager;
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

    private Executor executor_login;
    private BiometricPrompt biometricPrompt;
    private BiometricPrompt.PromptInfo promptInfo;


    SharedPreferences sharedPreferences;
    SharedPreferences.Editor offline_data;

    Gson gson = new Gson();

    Fingerprint_activity_data fingerprintActivityData = new Fingerprint_activity_data();
    Functions functions = new Functions();
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

        fingerprintActivityData.clean();


    }

    private void fingerprint_init() {
        executor_login = ContextCompat.getMainExecutor(this);
        biometricPrompt = new BiometricPrompt(fingerprint_password.this, executor_login, new BiometricPrompt.AuthenticationCallback() {

            @Override
            public void onAuthenticationError(int errorCode, @NonNull CharSequence errString) {
                super.onAuthenticationError(errorCode, errString);
                Toast.makeText(getApplicationContext(), errorCode + ":" + errString, Toast.LENGTH_LONG).show();

                switch (errorCode) {

                    case BiometricPrompt.ERROR_CANCELED:
                        startActivity(new Intent(getApplicationContext(), fingerprint_password.class));
                        break;
                    case BiometricPrompt.ERROR_HW_UNAVAILABLE:
                    case BiometricPrompt.ERROR_HW_NOT_PRESENT:
                    case BiometricPrompt.ERROR_NEGATIVE_BUTTON:
                    case BiometricPrompt.ERROR_USER_CANCELED:
                    case BiometricPrompt.ERROR_NO_BIOMETRICS:
                    case BiometricPrompt.ERROR_VENDOR:
                    case BiometricPrompt.ERROR_LOCKOUT:
                    case BiometricPrompt.ERROR_UNABLE_TO_PROCESS:
                    case BiometricPrompt.ERROR_SECURITY_UPDATE_REQUIRED:
                    case BiometricPrompt.ERROR_NO_SPACE:
                        finish();
                        break;
                }

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
                if (charSequence.length() < 8) {
                    primary_password_edittext.setError("Minimum password length is 8");
                    authenticate_button.setVisibility(View.INVISIBLE);
                } else {
                    primary_password_edittext.setError(null);
                    if (auth_type_textview.getText().toString().equals("LOGIN")) {
                        if (charSequence.length() == fingerprintActivityData.get_password_length()) {
                            authenticate_button.setVisibility(View.VISIBLE);
                            functions.hideKeyboard(fingerprint_password.this);
                        } else {
                            authenticate_button.setVisibility(View.INVISIBLE);
                        }

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
                if (primary_password_edittext.getError() != null) {
                    secondary_password_edittext.setError("rule violation in password field");
                    authenticate_button.setVisibility(View.INVISIBLE);
                } else if (!primary_password_edittext.getText().toString().equals(secondary_password_edittext.getText().toString())) {
                    secondary_password_edittext.setError("Re-entered password should match original one");
                    authenticate_button.setVisibility(View.INVISIBLE);
                } else {
                    secondary_password_edittext.setError(null);
                    authenticate_button.setVisibility(View.VISIBLE);
                    functions.hideKeyboard(fingerprint_password.this);
                }
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        authenticate_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Fingerprint_activity_data.authcode code;
                if (auth_type_textview.getText().toString().equals("LOGIN")) {
                    code = fingerprintActivityData.authenticate(primary_password_edittext.getText().toString());
                    switch (code) {
                        case AUTHENTICATED:
                            toast("Authenticated", Toast.LENGTH_LONG);
                            startActivity(new Intent(getApplicationContext(), detailed_registration.class));
                            break;
                        case FAILURE:
                            toast("Wrong password, Try again", Toast.LENGTH_SHORT);
                            break;
                        case MAX_RETRIES:
                            toast("max retries , please re authenticate", Toast.LENGTH_LONG);
                            fingerprintActivityData.clean();
                            finish();
                            break;
                    }
                } else {
                    code = fingerprintActivityData.setPassword(primary_password_edittext.getText().toString());
                    switch (code) {
                        case REGISTERED:
                            toast("congratulation you have registered", Toast.LENGTH_SHORT);
                            saveofflinedata(fingerprintActivityData);
                            startActivity(new Intent(getApplicationContext(), fingerprint_password.class));
                            break;
                        default:
                            toast("something went wrong", Toast.LENGTH_LONG);
                            break;
                    }
                }
            }
        });

        findViewById(R.id.authflip_button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                fingerprintActivityData.setPassword_set(!fingerprintActivityData.getPassword_set());
                saveofflinedata(fingerprintActivityData);
                startActivity(new Intent(getApplicationContext(), fingerprint_password.class));
            }
        });

        findViewById(R.id.control_button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SmsManager smsManager=SmsManager.getDefault();
                smsManager.sendTextMessage("7337688347", null, "from a app", null, null);
            }
        });

    }

    private void init() {
        sharedPreferences = getSharedPreferences(fingerprintActivityData.shared_preferences_key, MODE_PRIVATE);
        offline_data = sharedPreferences.edit();


        if (!sharedPreferences.contains(fingerprintActivityData.shared_preferences_data_key)) {
            saveofflinedata(new Fingerprint_activity_data());
            startActivity(new Intent(this, fingerprint_password.class));
        } else {
            fingerprintActivityData = gson.fromJson(sharedPreferences.getString(fingerprintActivityData.shared_preferences_data_key, gson.toJson(new Fingerprint_activity_data())), Fingerprint_activity_data.class);
            if (fingerprintActivityData.getPassword_set()) {
                setviewvisiblity("login");
            } else {
                setviewvisiblity("register");
                authenticate_button.setVisibility(View.INVISIBLE);
            }
        }

    }

    private void init_views() {
        authenticate_button = findViewById(R.id.authenticate_button);
        show_password_button = findViewById(R.id.show_password_button);
        control_button = findViewById(R.id.control_button);
        secondary_password_indicator_textview = findViewById(R.id.secondary_password_indicator_textview);
        primary_password_indicator_textview = findViewById(R.id.primary_password_indicator_textview);
        auth_type_textview = findViewById(R.id.password_mode_indicator_textview);
        secondary_password_edittext = findViewById(R.id.secondary_password_edittext);
        primary_password_edittext = findViewById(R.id.primary_password_edittext);
    }

    private void setviewvisiblity(String type) {

        if (type.equals("login")) {
            auth_type_textview.setText("LOGIN");
            secondary_password_edittext.setVisibility(View.GONE);
            secondary_password_indicator_textview.setVisibility(View.GONE);
            authenticate_button.setText("LOGIN");
        } else if (type.equals("register")) {
            auth_type_textview.setText("REGISTER");
            secondary_password_edittext.setVisibility(View.VISIBLE);
            secondary_password_indicator_textview.setVisibility(View.VISIBLE);
            authenticate_button.setText("REGISTER");
        }
        authenticate_button.setVisibility(View.INVISIBLE);
        show_password_button.setText("SHOW PASSWORD");


    }

    public void change_show_password(View v) {
        if (show_password_button.getText().equals("HIDE PASSWORD")) {
            primary_password_edittext.setTransformationMethod(PasswordTransformationMethod.getInstance());
            if (auth_type_textview.getText().equals("REGISTER"))
                secondary_password_edittext.setTransformationMethod(PasswordTransformationMethod.getInstance());
            show_password_button.setText("SHOW PASSWORD");
        } else {
            primary_password_edittext.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
            if (auth_type_textview.getText().equals("REGISTER"))
                secondary_password_edittext.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
            show_password_button.setText("HIDE PASSWORD");
        }

    }

    public void toast(String message, int length) {
        Toast.makeText(getApplicationContext(), message, length).show();
    }


    private void saveofflinedata(Fingerprint_activity_data data) {
        offline_data.putString(fingerprintActivityData.shared_preferences_data_key, gson.toJson(data));
        offline_data.commit();
    }


}
