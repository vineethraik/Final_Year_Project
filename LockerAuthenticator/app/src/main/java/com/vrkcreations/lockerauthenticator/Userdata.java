package com.vrkcreations.lockerauthenticator;

public class Userdata {
    public final String shared_preferences_data_key = "User_data";
    public final String shared_preferences_key = "Offline_data";

    private boolean Registered =false;
    private boolean Idset=false;
    private String Id="";
    private String Name="";
    private String Nickname="";
    private String Phone="0011001100";
    private String Email="vrk@temo.c";


    public boolean isRegistered() {
        return Registered;
    }

    public void setRegistered(boolean registered) {
        this.Registered = registered;
    }

    public boolean isIdset() {
        return Idset;
    }

    public void setIdset(boolean idset) {
        Idset = idset;
    }

    public String getName(){
        return Name;
    }

    public String getNickname() {
        return Nickname;
    }

    public void setName(String name) {
        Name = name;
    }

    public void setNickname(String nickname) {
        Nickname = nickname;
    }

    public String getEmail() {
        return Email;
    }

    public void setEmail(String email) {
        Email = email;
    }

    public String getPhone() {
        return Phone;
    }

    public void setPhone(String phone) {
        Phone = phone;
    }

    public String getId() {
        return Id;
    }

    public void setId(String id) {
        Id = id;
    }


}

