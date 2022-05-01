package com.vrkcreations.lockerauthenticator;

import java.text.SimpleDateFormat;
import java.util.Date;

public class Locker {
    private String Id="";
    private String Name="";
    private String Totp_base_hash="";
    Functions functions=new Functions();

    public Locker(String id, String name, String totp_base_hash) {
        Id = id;
        Name = name;
        Totp_base_hash = totp_base_hash;
    }

    public String getId() {
        return Id;
    }

    public void setId(String id) {
        Id = id;
    }

    public String getName() {
        return Name;
    }

    public void setName(String name) {
        Name = name;
    }

    public void setTotp_base_hash(String totp_base_hash) {
        Totp_base_hash = totp_base_hash;
    }


    public String get_TOTP(String otp){
        SimpleDateFormat simpleDateFormat=new SimpleDateFormat("mm:HH:dd:MM:yyyy");
        return functions.get_pin_from_hex(functions.SHA1(Totp_base_hash+":"+otp+simpleDateFormat.format(new Date())));
    }

}
