package com.eli.find;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

import com.googlecode.tesseract.android.TessBaseAPI;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

/**
 * Created by wangchuangbin on 2019/6/14.
 */
public class InitTask extends AsyncTask<Void, Void, Boolean> {
    private Context context;
    private TessBaseAPI tessBaseAPI;
    private String language = "eng";
    private static String TAG = "InitTask";

    public InitTask(Context context, TessBaseAPI tessBaseAPI) {
        this.context = context;
        this.tessBaseAPI = tessBaseAPI;
    }

    @Override
    protected Boolean doInBackground(Void... voids) {
        System.loadLibrary("OpenCV");
        InputStream is = null;
        FileOutputStream fos = null;
        try {
            is = context.getAssets().open(language + ".traineddata");
            File file = new File("/sdcard/tess/tessdata/" + language + ".traineddata");
            if (!file.exists()) {
                file.getParentFile().mkdirs();
                fos = new FileOutputStream(file);
                byte[] buffer = new byte[2048];
                int len;
                while ((len = is.read(buffer)) != -1) {
                    fos.write(buffer, 0, len);
                }
                fos.close();
            }
            is.close();
            return tessBaseAPI.init("/sdcard/tess", language);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (null != is) is.close();
                if (null != fos) fos.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return false;
    }

    @Override
    protected void onPostExecute(Boolean aBoolean) {
        if (aBoolean) {
            FindInIDCardUtil.isInitSuccess = true;
        } else {
            Log.e(TAG, "初始化失败");
        }
    }
}
