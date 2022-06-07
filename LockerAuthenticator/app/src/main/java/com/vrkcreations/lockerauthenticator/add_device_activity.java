package com.vrkcreations.lockerauthenticator;


import androidx.annotation.Keep;
import androidx.appcompat.app.AppCompatActivity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.Cache;
import com.android.volley.Network;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.RetryPolicy;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.BasicNetwork;
import com.android.volley.toolbox.DiskBasedCache;
import com.android.volley.toolbox.HurlStack;
import com.android.volley.toolbox.StringRequest;
import com.google.gson.Gson;

@Keep public class add_device_activity extends AppCompatActivity {

    private RequestQueue requestQueue;

    Button sync_button;
    Button add_device_button;
    TextView connection_status_textview;
    TextView device_id_textview;
    TextView device_name_textview;
    EditText secret_code_edittext;
    EditText display_name_edittext;

    enum c_code {PING,ID,NAME,REGISTER,EMPTY,IS_REG,ADD_ID};

    Userdata userdata = new Userdata();
    Locker locker=new Locker();
    Functions functions = new Functions();
    SharedPreferences sharedPreferences;
    SharedPreferences.Editor offline_data;
    Gson gson = new Gson();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_device);
        init_views();
        init_volley();
        init();
        init_listener();

    }

    @Override
    protected void onStop() {
        super.onStop();
        if (requestQueue != null) {
            requestQueue.cancelAll("tag");
        }
    }


    @Keep private void init_volley() {
        Cache cache = new DiskBasedCache(getCacheDir(), 1024 * 1024);
        Network network = new BasicNetwork(new HurlStack());
        requestQueue = new RequestQueue(cache, network);
        requestQueue.start();
    }

    private void init_views(){
        sync_button=findViewById(R.id.sync_button);
        add_device_button=findViewById(R.id.add_device_button);
        connection_status_textview=findViewById(R.id.sync_indicator_textview);
        device_id_textview=findViewById(R.id.device_id_textview);
        device_name_textview=findViewById(R.id.device_name_textview);
        display_name_edittext=findViewById(R.id.display_name_edittext);
        secret_code_edittext=findViewById(R.id.secret_code_edittext);
    }

    private void init(){
        sharedPreferences = getSharedPreferences(userdata.shared_preferences_key, MODE_PRIVATE);
        offline_data = sharedPreferences.edit();
        userdata = gson.fromJson(sharedPreferences.getString(userdata.shared_preferences_data_key, gson.toJson(new Userdata())), Userdata.class);
    }

    private void init_listener(){
        sync_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                internet_request("http://192.168.4.1/ping",c_code.PING);
                            }
        });

        add_device_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                register();
                }
        });
    }

    @Keep private void internet_request(String url, c_code tag){
        Log.d("test","url:-"+url);
        StringRequest stringRequest=new StringRequest(Request.Method.GET, url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                Log.d("test","response:"+response);
                switch (tag){
                    case PING:
                        connection_status_textview.setText("online");
                        internet_request("http://192.168.4.1/info?q=id",c_code.ID);
                        break;
                    case ID:
                        device_id_textview.setText(response);
                        locker.setId(response);
                        internet_request("http://192.168.4.1/info?q=name",c_code.NAME);
                        break;
                    case NAME:
                        device_name_textview.setText(response);
                        locker.setName(response);
                        internet_request("http://192.168.4.1/add-user?target=id&id="+userdata.getId(),c_code.ADD_ID);
                        break;
                    case ADD_ID:
                        if(response.equals("no_user_space_free")){
                            Toast.makeText(getApplicationContext(),"No extra user can be added to this Locker",Toast.LENGTH_LONG).show();
                            finish();
                        }else{
                        internet_request("http://192.168.4.1/add-user?target=user_info&id="+userdata.getId()
                                        +"&name="+userdata.getName()
                                        +"&email="+userdata.getEmail()
                                        +"&nickname="+userdata.getNickname()
                                        +"&phone="+userdata.getPhone(),
                                c_code.REGISTER );}
                        break;
                    case REGISTER:
                        break;
                    case IS_REG:
                        if(response.equals("YES")){
                            userdata.devices.add(locker);
                            Toast.makeText(getApplicationContext(), "registered", Toast.LENGTH_LONG).show();
                            saveofflinedata();
                            startActivity(new Intent(getApplicationContext(),MainActivity.class).setFlags(Intent.FLAG_ACTIVITY_NO_HISTORY));
                            finish();
                        }else{
                            Toast.makeText(getApplicationContext(), "not registered", Toast.LENGTH_LONG).show();
                        }
                    default:
                        break;
                }

            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                switch (tag){
                    case PING:
                        connection_status_textview.setText("offline");
                        break;
                    case ID:
                        Log.d("test","error fetching id"+error.getMessage());
                        break;
                    case NAME:
                        Log.d("test","error fetching name"+error.getMessage());
                        break;
                    case REGISTER:
                        Log.d("test","error registering"+error.getMessage());
                    default:
                        break;
                }
            }
        });

        stringRequest.setTag("tag");
        if(tag==c_code.REGISTER){
            stringRequest.setRetryPolicy(new RetryPolicy() {
                @Override
                public int getCurrentTimeout() {
                    return 60000;
                }

                @Override
                public int getCurrentRetryCount() {
                    return 0;
                }

                @Override
                public void retry(VolleyError error) throws VolleyError {
                    Log.d("test","retry error:-"+error.getMessage());
                }
            });
        }
        requestQueue.add(stringRequest);
    }

    private void saveofflinedata() {
        saveofflinedata(userdata);
    }

    private void saveofflinedata(Userdata data) {
        Log.d("test","from saveofflinedata:--"+data.toString());
        offline_data.putString(userdata.shared_preferences_data_key, gson.toJson(data));
        offline_data.commit();
    }

    private void register(){

            locker.setTotp_base_hash(functions.SHA1(secret_code_edittext.getText().toString()));
            if(!display_name_edittext.getText().toString().equals("")){
                locker.setName(display_name_edittext.getText().toString());
            }

            new AlertDialog.Builder(this)
                    .setTitle("ADD DEVICE")
                    .setMessage("code:"+functions.get_pin_from_hex(functions.SHA1(secret_code_edittext.getText().toString())))
                    .setPositiveButton("Register", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialogInterface, int i) {
                            internet_request("http://192.168.4.1/info?q=is_registered&id=" + userdata.getId(), c_code.IS_REG);
                        }
                    })
                    .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialogInterface, int i) {
                            finish();
                        }
                    }).show();

    }
}