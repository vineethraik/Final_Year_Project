package com.vrkcreations.lockerauthenticator;

public class Locker {
    public String Id="";
    private String Totp_base_hash="";
    Functions functions=new Functions();

    public void setTotp_base_hash(String totp_base_hash) {
        Totp_base_hash = totp_base_hash;
    }
    public String getTOTP(int otp,int min,int hour,int date,int month,int year){
        return functions.get_pin_from_hex(functions.SHA1(Totp_base_hash+":"+otp+":"+min+":"+hour+":"+date+":"+month+":"+year));

    }

}
