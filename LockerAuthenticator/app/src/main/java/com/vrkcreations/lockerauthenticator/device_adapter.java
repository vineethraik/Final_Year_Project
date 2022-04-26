package com.vrkcreations.lockerauthenticator;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.ArrayList;

public class device_adapter extends ArrayAdapter<Locker> {
    public device_adapter(Context context, ArrayList<Locker> device_arraylist){
        super(context,R.layout.device_view,device_arraylist);

    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        Locker device= getItem(position);

        if(convertView==null){
            convertView= LayoutInflater.from(getContext()).inflate(R.layout.device_view,parent,false);
        }
        TextView id_text=convertView.findViewById(R.id.device_id);
        TextView name_text=convertView.findViewById(R.id.device_name);

        id_text.setText(device.getId());
        name_text.setText(device.getName());


        return convertView;
    }
}
