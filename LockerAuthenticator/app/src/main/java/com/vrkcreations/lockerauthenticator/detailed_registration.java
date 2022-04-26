package com.vrkcreations.lockerauthenticator;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.android.volley.Cache;
import com.android.volley.Network;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.BasicNetwork;
import com.android.volley.toolbox.DiskBasedCache;
import com.android.volley.toolbox.HurlStack;
import com.android.volley.toolbox.StringRequest;
import com.google.gson.Gson;

import java.util.Random;
import java.util.Timer;

public class detailed_registration extends AppCompatActivity {
    private RequestQueue requestQueue;

    private int mail_otp = 0;
    private int phone_otp = 0;
    Timer resend_otp_timer=new Timer();

    Button send_otp_button;
    Button register_button;
    EditText phone_edittext;
    EditText email_edittext;
    EditText name_edittext;
    EditText nickname_edittext;
    EditText phone_otp_edittext;
    EditText email_otp_edittext;


    Userdata userdata = new Userdata();
    Functions functions = new Functions();
    SharedPreferences sharedPreferences;
    SharedPreferences.Editor offline_data;
    Gson gson = new Gson();

    enum urlcode {OTP_PHONE, OTP_MAIL}

    /**
     * register button
     *
     * **/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_detailed_registration);

        init_volley();
        init_views();
        listeners();
        init();

    }

    @Override
    protected void onStop() {
        super.onStop();
        if (requestQueue != null) {
            requestQueue.cancelAll("tag");
        }
    }

    private void init_volley() {
        Cache cache = new DiskBasedCache(getCacheDir(), 1024 * 1024);
        Network network = new BasicNetwork(new HurlStack());
        requestQueue = new RequestQueue(cache, network);
        requestQueue.start();
    }

    private void init_views() {
        send_otp_button = findViewById(R.id.send_otp_button);
        register_button = findViewById(R.id.register_button);
        phone_edittext = findViewById(R.id.phone_edittext);
        email_edittext = findViewById(R.id.mail_edittext);
        name_edittext = findViewById(R.id.name_edittext);
        nickname_edittext = findViewById(R.id.nickname_edittext);
        phone_otp_edittext = findViewById(R.id.phone_otp_edittext);
        email_otp_edittext = findViewById(R.id.email_otp_edittext);
    }

    private void init() {
        send_otp_button.setEnabled(false);
        register_button.setEnabled(false);

        sharedPreferences = getSharedPreferences(userdata.shared_preferences_key, MODE_PRIVATE);
        offline_data = sharedPreferences.edit();
        if (!sharedPreferences.contains(userdata.shared_preferences_data_key)) {
            saveofflinedata(new Userdata());

        }

        userdata = gson.fromJson(sharedPreferences.getString(userdata.shared_preferences_data_key, gson.toJson(new Userdata())), Userdata.class);
        Log.d("test","inputting:--"+userdata.toString());
        if (userdata.isRegistered()) {
            startActivity(new Intent(getApplicationContext(), MainActivity.class));
            finish();
        }


    }

    private void listeners() {
        name_edittext.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if (charSequence.length() >= 20) {
                    name_edittext.setError("Name is too long");
                } else {
                    name_edittext.setError(null);
                }
                check_and_enable_buttons();
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
        nickname_edittext.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if (charSequence.toString().contains(" ") || charSequence.toString().contains("\\")) {
                    nickname_edittext.setError("Invalid characters");
                } else if (charSequence.length() >= 20) {
                    nickname_edittext.setError("Nickname is too long");
                } else {
                    nickname_edittext.setError(null);
                }
                check_and_enable_buttons();
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
        phone_edittext.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if (charSequence.length() != 10) {
                    phone_edittext.setError("Invalid phone number");
                } else {
                    phone_edittext.setError(null);
                }
                check_and_enable_buttons();
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
        email_edittext.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if (!charSequence.toString().contains("@") || !charSequence.toString().contains(".")) {
                    email_edittext.setError("Invalid email id");
                } else {
                    email_edittext.setError(null);
                }
                check_and_enable_buttons();
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
        phone_otp_edittext.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if (charSequence.length() != 6) {
                    phone_otp_edittext.setError("Invalid OTP");
                } else {
                    phone_otp_edittext.setError(null);
                }
                check_and_enable_buttons();
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
        email_otp_edittext.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if (charSequence.length() != 6) {
                    email_otp_edittext.setError("Invalid OTP");
                } else {
                    email_otp_edittext.setError(null);
                }
                check_and_enable_buttons();
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        send_otp_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                functions.hideKeyboard(detailed_registration.this);
                send_otp_button.setEnabled(false);
                Random random = new Random();
                phone_otp = 100000 + random.nextInt(900000);
                mail_otp = 100000 + random.nextInt(900000);
                internet_request(make_url(urlcode.OTP_PHONE, phone_edittext.getText().toString().trim(), Integer.toString(phone_otp), "Authenticator_App"), urlcode.OTP_PHONE);
                internet_request(make_url(urlcode.OTP_MAIL, email_edittext.getText().toString().trim(), Integer.toString(mail_otp), "Authenticator_App"), urlcode.OTP_MAIL);
            }
        });
        register_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(Integer.toString(mail_otp).equals(email_otp_edittext.getText().toString())
                    &&
                    Integer.toString(phone_otp).equals(phone_otp_edittext.getText().toString())
                ){
                    userdata.setName(name_edittext.getText().toString());
                    userdata.setNickname(nickname_edittext.getText().toString());
                    userdata.setPhone(phone_edittext.getText().toString());
                    userdata.setEmail(email_edittext.getText().toString());
                    userdata.setRegistered(true);
                    saveofflinedata(userdata);
                    startActivity(new Intent(getApplicationContext(),MainActivity.class));
                    finish();
                }else{
                    toast("Wrong otp",Toast.LENGTH_LONG);
                    phone_otp_edittext.setText("");
                    email_otp_edittext.setText("");
                }
            }
        });
    }

    private void internet_request(String url, urlcode tag) {
        StringRequest stringRequest = new StringRequest(Request.Method.GET, url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                switch (tag) {

                    case OTP_PHONE:
                        if (response.contains("locker_otp_mobile")) {
                            toast("otp for mobile is sent", Toast.LENGTH_LONG);
                        }
                        break;
                    case OTP_MAIL:
                        if (response.contains("locker_otp_mail")) {
                            toast("otp for mail is sent", Toast.LENGTH_LONG);
                        }
                        break;
                }

            }
        }
                ,
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        send_otp_button.setEnabled(true);
                        switch (tag) {
                            case OTP_PHONE:
                                toast("phone_error:" + error.getMessage(), Toast.LENGTH_LONG);
                                break;
                            case OTP_MAIL:
                                toast("mail_error:" + error.getMessage(), Toast.LENGTH_LONG);
                                break;
                        }

                    }
                });
        stringRequest.setTag("tag");
        requestQueue.add(stringRequest);

    }

    public void toast(String message, int length) {
        Toast.makeText(getApplicationContext(), message, length).show();
    }

    private String make_url(urlcode code, String value1, String value2, String value3) {
        String result;
        String event = "test";
        switch (code) {
            case OTP_PHONE:
                event = "locker_otp_mobile";
                break;
            case OTP_MAIL:
                event = "locker_otp_mail";
                break;
        }
        result = "https://maker.ifttt.com/trigger/";
        result += event;
        result += "/with/key/";
        result += functions.get_web_key();
        result += "?value1=";
        result += value1;
        result += "&value2=";
        result += value2;
        result += "&value3=";
        result += value3;
        //toast(result,Toast.LENGTH_LONG);
        return result;


    }

    private void check_and_enable_buttons() {
        if (name_edittext.getText().toString().equals("")
                ||
                nickname_edittext.getText().toString().equals("")
                ||
                phone_edittext.getText().toString().equals("")
                ||
                email_edittext.getText().toString().equals("")
                ||
                !(name_edittext.getError() == null)
                ||
                !(nickname_edittext.getError() == null)
                ||
                !(phone_edittext.getError() == null)
                ||
                !(email_edittext.getError() == null)
        ) {
            send_otp_button.setEnabled(false);
            register_button.setEnabled(false);
        } else if (phone_otp_edittext.getText().toString().equals("")
                ||
                email_otp_edittext.getText().toString().equals("")
                ||
                !(phone_otp_edittext.getError() == null)
                ||
                !(email_otp_edittext.getError() == null)
        ) {
            send_otp_button.setEnabled(true);
            register_button.setEnabled(false);
        }else{
            send_otp_button.setEnabled(true);
            register_button.setEnabled(true);
        }
    }

    private void saveofflinedata(Userdata data) {
        Log.d("test","from saveofflinedata:--"+data.toString());
        offline_data.putString(userdata.shared_preferences_data_key, gson.toJson(data));
        offline_data.commit();
    }
}