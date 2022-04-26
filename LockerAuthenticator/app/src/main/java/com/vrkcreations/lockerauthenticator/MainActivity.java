package com.vrkcreations.lockerauthenticator;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;

import java.util.ArrayList;
import java.util.zip.Inflater;

public class MainActivity extends AppCompatActivity {


    LayoutInflater layoutInflater;
    ArrayList<Locker> lc_list=new ArrayList<>();
    device_adapter adapter;
    Locker temp_locker;

    ListView device_list;

    Userdata userdata = new Userdata();
    Functions functions = new Functions();
    SharedPreferences sharedPreferences;
    SharedPreferences.Editor offline_data;
    Gson gson = new Gson();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        layoutInflater=getLayoutInflater();
        init_view();
        init();
        listeners();
    }

    void init(){
        sharedPreferences = getSharedPreferences(userdata.shared_preferences_key, MODE_PRIVATE);
        offline_data = sharedPreferences.edit();
        if (!sharedPreferences.contains(userdata.shared_preferences_data_key)) {
           Toast.makeText(getApplicationContext(),"some error",Toast.LENGTH_LONG).show();
           finish();
        }

        userdata = gson.fromJson(sharedPreferences.getString(userdata.shared_preferences_data_key,gson.toJson(new Userdata())), Userdata.class);

        if(userdata.devices.size()<1){
            lc_list.clear();
            lc_list.add(new Locker("000001","DUMMY","vineethraik"));
            userdata.devices=lc_list;
            saveofflinedata(userdata);
        }else{
            lc_list=userdata.devices;
        }
        adapter=new device_adapter(MainActivity.this,lc_list);
        device_list.setAdapter(adapter);
    }

    void init_view(){
        device_list=findViewById(R.id.device_listview);
    }

    void listeners(){
        device_list.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                startActivity(new Intent(getApplicationContext(),MainActivity.class).putExtra("locker_data",gson.toJson(userdata.devices.get(i))));
                Toast.makeText(getApplicationContext(),"position:"+i,Toast.LENGTH_LONG).show();
            }
        });
    }

    private void saveofflinedata(Userdata data) {
        Log.d("test","from saveofflinedata:--"+data.toString());
        offline_data.putString(userdata.shared_preferences_data_key, gson.toJson(data));
        offline_data.commit();
    }
}