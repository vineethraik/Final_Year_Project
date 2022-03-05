package com.vrkcreations.lockerauthenticator;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
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

import java.util.Random;

public class detailed_registration extends AppCompatActivity {
    private RequestQueue requestQueue;
    private String webhook_key = "nSR1qlvFpETll-TwVujtKdrvxlXIlB86WRTL82hpRiw";
    private int mail_otp = 0;
    private int mobile_otp = 0;

    Button send_otp_button;
    EditText phone_edittext;
    EditText mail_edittext;

    enum urlcode {OTP_PHONE, OTP_MAIL}

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
        phone_edittext = findViewById(R.id.phone_edittext);
        mail_edittext = findViewById(R.id.mail_edittext);
    }

    private void init() {

    }

    private void listeners() {
        send_otp_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Random random = new Random();
                mobile_otp = 100000 + random.nextInt(900000);
                mail_otp = 100000 + random.nextInt(900000);
                internet_request(make_url(urlcode.OTP_PHONE, phone_edittext.getText().toString().trim(),Integer.toString(mobile_otp),"Authenticator_App" ),urlcode.OTP_PHONE);
                internet_request(make_url(urlcode.OTP_MAIL,mail_edittext.getText().toString().trim(),Integer.toString(mail_otp),"Authenticator_App"),urlcode.OTP_MAIL);
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

                        switch (tag) {

                            case OTP_PHONE:
                                toast("phoneerror:"+error.getMessage(),Toast.LENGTH_LONG);
                                break;
                            case OTP_MAIL:
                                toast("mailerror:"+error.getMessage(),Toast.LENGTH_LONG);
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
        result+=event;
        result+="/with/key/";
        result+=webhook_key;
        result+="?value1=";
        result+=value1;
        result+="&value2=";
        result+=value2;
        result+="&value3=";
        result+=value3;
        toast(result,Toast.LENGTH_LONG);
        return result;


    }
}