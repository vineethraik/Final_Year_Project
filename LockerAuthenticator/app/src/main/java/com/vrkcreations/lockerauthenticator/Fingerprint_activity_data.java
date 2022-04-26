package com.vrkcreations.lockerauthenticator;

import android.util.Log;
import android.widget.Toast;

public class Fingerprint_activity_data {
    public final String shared_preferences_data_key = "Fingerprint_activity_data";
    public final String shared_preferences_key = "Offline_data";

    public enum authcode{
        AUTHENTICATED ,
        FAILURE,
        MAX_RETRIES,
        REGISTERED
    }

    private boolean password_set=false;
    private String password;
    private int failed_try_count=0;
    public int maxtries=3;

    public void clean(){
        failed_try_count=0;
    }

    public Boolean getPassword_set() {
        return password_set;
    }

    public int get_password_length(){return password.length();}

    public void setPassword_set(boolean password_set){
        this.password_set=password_set;
    }

    public authcode setPassword(String password) {
        this.password = password;
        password_set=true;
        return authcode.REGISTERED;
    }

    public authcode authenticate(String password){

        if(this.password.equals(password)){

            return authcode.AUTHENTICATED;
        }
        else{
            failed_try_count++;
            if(failed_try_count>=maxtries){
                return authcode.MAX_RETRIES;
            }
            return authcode.FAILURE;
        }

    }


}
