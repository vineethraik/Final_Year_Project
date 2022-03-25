package com.vrkcreations.lockerauthenticator;

import android.app.Activity;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Switch;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Functions {
    public void hideKeyboard(Activity activity) {
        InputMethodManager imm = (InputMethodManager) activity.getSystemService(Activity.INPUT_METHOD_SERVICE);
        //Find the currently focused view, so we can grab the correct window token from it.
        View view = activity.getCurrentFocus();
        //If no view currently has focus, create a new one, just so we can grab a window token from it
        if (view == null) {
            view = new View(activity);
        }
        imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }


    // functions for sha1 starts
    public String SHA1(String message){
        String hash="";
        try {
            MessageDigest messageDigest=MessageDigest.getInstance( "SHA-1" );
            byte[] bytes=message.getBytes(StandardCharsets.UTF_8);
            messageDigest.update(bytes,0, bytes.length);
            bytes=messageDigest.digest();
            hash=bytesToHex(bytes);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }

        return hash;
    }

    final private static char[] hexArray = "0123456789ABCDEF".toCharArray();
    private static String bytesToHex( byte[] bytes )
    {
        char[] hexChars = new char[ bytes.length * 2 ];
        for( int j = 0; j < bytes.length; j++ )
        {
            int v = bytes[ j ] & 0xFF;
            hexChars[ j * 2 ] = hexArray[ v >>> 4 ];
            hexChars[ j * 2 + 1 ] = hexArray[ v & 0x0F ];
        }
        return new String( hexChars );
    }
    // functions for sha1 ends

    public String get_pin_from_hex(String hex){
        int _pin[]={0,0,0,0,0,0};
        char C[]=hex.toCharArray();
        String pin="";
        for (int i=0;i<hex.length();i++){
            switch(C[i]){
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                    _pin[i%6]+=C[i]-48;
                    break;
                case 'A':
                    _pin[i%6]+=10;
                    break;
                case 'B':
                    _pin[i%6]+=11;
                    break;
                case 'C':
                    _pin[i%6]+=12;
                    break;
                case 'D':
                    _pin[i%6]+=13;
                    break;
                case 'E':
                    _pin[i%6]+=14;
                    break;
                case 'F':
                    _pin[i%6]+=15;
                    break;
            }
            _pin[i%6]=_pin[i%6]%10;

        }

        for (int i=0;i<_pin.length;i++){
            pin+=Integer.toString(_pin[i]);
        }

        return pin;
    }

}
