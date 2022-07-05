package com.vrkcreations.lockerauthenticator;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.gson.Gson;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

public class locker_activity extends AppCompatActivity {
    Intent intent;
    Locker locker;
    Timer timer=new Timer();
    TimerTask timerTask;
    int time_left=0;

    TextView locker_id_textview;
    TextView locker_name_textview;
    EditText OTP_textview;
    Button generate_TOTP_button;
    TextView totp_textview;
    TextView time_left_textview;
    View totp_layout;
    View timeleft_layout;


    Userdata userdata = new Userdata();
    Functions functions = new Functions();
    SharedPreferences sharedPreferences;
    SharedPreferences.Editor offline_data;
    Gson gson = new Gson();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_locker);
        init_views();
        init();
        init_listeners();
    }

    private void init(){
        intent=getIntent();
        locker=gson.fromJson(intent.getStringExtra("locker_data"),Locker.class);
        locker_id_textview.setText(locker.getId());
        locker_name_textview.setText(locker.getName());

        generate_TOTP_button.setEnabled(false);

        totp_layout.setVisibility(View.INVISIBLE);
        timeleft_layout.setVisibility(View.INVISIBLE);


    }

    private void init_views(){
        locker_id_textview=findViewById(R.id.locker_id);
        locker_name_textview=findViewById(R.id.locker_name);
        OTP_textview=findViewById(R.id.otp_edittext);
        generate_TOTP_button=findViewById(R.id.generate_totp_button);
        totp_textview=findViewById(R.id.totp_textview);
        time_left_textview=findViewById(R.id.timeleft_textview);

        totp_layout=findViewById(R.id.totp_show_layout);
        timeleft_layout=findViewById(R.id.time_left_layout);

    }

    private void init_listeners() {
        OTP_textview.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                if(charSequence.length()==6){
                    generate_TOTP_button.setEnabled(true);
                }else{
                    generate_TOTP_button.setEnabled(false);
                }
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });
        generate_TOTP_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                timerTask=new TimerTask() {
                    @Override
                    public void run() {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                SimpleDateFormat sdf=new SimpleDateFormat("ss");
                                time_left=60- new Integer(sdf.format(new Date()));
                                totp_textview.setText(locker.get_TOTP(OTP_textview.getText().toString()));
                                time_left_textview.setText(time_left+" seconds");
                                totp_layout.setVisibility(View.VISIBLE);
                                timeleft_layout.setVisibility(View.VISIBLE);
                            }
                        });
                    }
                };
                timer.schedule(timerTask,0,1000);
            }
        });
    }



    @Override
    protected void onDestroy() {
        super.onDestroy();
        timer.cancel();
    }

    @Override
    protected void onStop() {
        super.onStop();
       // startActivity(new Intent(getApplicationContext(),MainActivity.class));
        finish();
    }
}