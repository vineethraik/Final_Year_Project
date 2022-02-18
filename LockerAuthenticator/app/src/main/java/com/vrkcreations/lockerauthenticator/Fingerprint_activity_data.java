package com.vrkcreations.lockerauthenticator;

public class Fingerprint_activity_data {
    public enum authcode{
        AUTHENTICATED ,
        FAILURE,
        MAX_RETRIES
    }

    private Boolean password_set=false;
    private String password;
    private int failed_try_count=0;

    public void clean(){
        failed_try_count=0;
    }

    public Boolean getPassword_set() {
        return password_set;
    }

    public void setPassword_set(Boolean password_set) {
        this.password_set = password_set;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public authcode authenticate(String password){
        if(this.password==password){
            return authcode.AUTHENTICATED;
        }
        else{
            failed_try_count++;
            if(failed_try_count<=3){
                return authcode.MAX_RETRIES;
            }
            return authcode.FAILURE;
        }

    }
}
