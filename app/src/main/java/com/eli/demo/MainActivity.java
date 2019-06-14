package com.eli.demo;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.eli.find.FindInIDCardUtil;

public class MainActivity extends AppCompatActivity {
    private TextView tvIdCardNum;
    private ImageView ivIDCard;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        init();
        tvIdCardNum = findViewById(R.id.tvIDCardNum);
        ivIDCard = findViewById(R.id.ivIDCard);
    }

    public void onClick(View view) {
//        String idCardNum = FindInIDCardUtil.findIDCardNum(BitmapFactory.decodeResource(getResources(), R.drawable.id_card_2));
        String idCardNum = FindInIDCardUtil.findIDCardNum(ivIDCard);
        tvIdCardNum.setText(idCardNum);
    }

    private void init() {
        //权限申请
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!(getPackageManager().checkPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE, getPackageName()) == PackageManager.PERMISSION_GRANTED)) {
                requestPermissions(new String[]{
                        Manifest.permission.WRITE_EXTERNAL_STORAGE
                }, 1981);
            }
        }
        FindInIDCardUtil.init(this);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        FindInIDCardUtil.init(this);
    }
}
