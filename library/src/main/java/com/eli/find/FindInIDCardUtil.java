package com.eli.find;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.text.TextUtils;
import android.widget.ImageView;

import com.googlecode.tesseract.android.TessBaseAPI;

/**
 * Created by wangchuangbin on 2019/6/14.
 */
public class FindInIDCardUtil {
    private static TessBaseAPI tessBaseAPI;
    protected static boolean isInitSuccess = false;

    public static void init(Context context) {
        if (isInitSuccess) return;
        if (tessBaseAPI == null) {
            tessBaseAPI = new TessBaseAPI();
        }
        new InitTask(context, tessBaseAPI).execute();
    }

    public static String findIDCardNum(ImageView imageView) {
        try {
            if (!isInitSuccess) return "";
            Bitmap bitmap = ((BitmapDrawable) imageView.getDrawable()).getBitmap();
            String idNumber = findIDCardNum(bitmap);
            return idNumber;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public static String findIDCardNum(Bitmap bitmap) {
        try {
            if (!isInitSuccess) return "";
            Bitmap bitmapNum = findIdNumber(bitmap, bitmap.getConfig());
            if (bitmapNum != null) {
                tessBaseAPI.setImage(bitmapNum);
                String idNumber = tessBaseAPI.getUTF8Text();
                if (bitmapNum != null) {
                    bitmapNum.recycle();
                    bitmapNum = null;
                }
                if (!TextUtils.isEmpty(idNumber) && idNumber.length() == 18) {
                    return idNumber;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }


    private static native Bitmap findIdNumber(Bitmap bitmap, Bitmap.Config argb8888);

}
