package com.vrkcreations.lockerauthenticator;

import androidx.appcompat.app.AppCompatActivity;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

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

public class add_device_activity extends AppCompatActivity {

    private RequestQueue requestQueue;

    Button sync_button;
    Button add_device_button;
    TextView connection_status_textview;
    TextView device_id_textview;
    TextView device_name_textview;
    EditText secret_code_edittext;
    EditText display_name_edittext;

    enum c_code {PING,ID,NAME,};

    Userdata userdata = new Userdata();
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

    private void init_volley() {
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
    }

    private void init_listener(){

    }

    private void internet_request(String url,c_code tag){
        StringRequest stringRequest=new StringRequest(Request.Method.GET, url, new Response.Listener<String>() {
            @Override
            public void onResponse(String response) {
                switch (tag){
                    case PING:
                        break;
                    default:
                        break;
                }

            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                switch (tag){
                    case PING:
                        break;
                    default:
                        break;
                }
            }
        });
    }

    private void saveofflinedata() {
        saveofflinedata(userdata);
    }

    private void saveofflinedata(Userdata data) {
        Log.d("test","from saveofflinedata:--"+data.toString());
        offline_data.putString(userdata.shared_preferences_data_key, gson.toJson(data));
        offline_data.commit();
    }
}