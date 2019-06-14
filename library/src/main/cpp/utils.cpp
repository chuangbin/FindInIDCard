#define LOG_TAG "Utils"

#include <android/bitmap.h>

#include "utils.h"
#include "common.h"

//将bitmap转为矩阵
void bitmap2Mat(JNIEnv *env, jobject bitmap, Mat *mat, bool needPremultiplyAlpha) {
    AndroidBitmapInfo info;
    void *pixels = 0;
    Mat &dst = *mat;
    //bitmap信息
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0)
        return;
    //必须是 rgba8888 rgb565
    if (!(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
          info.format == ANDROID_BITMAP_FORMAT_RGB_565))
        return;
    //锁定位图以获取缓冲区
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0)
        return;
    if (!pixels)
        return;

    dst.create(info.height, info.width, CV_8UC4);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGI("bitmap2Map:ANDROID_BITMAP_FORMAT_RGBA_8888");
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if (needPremultiplyAlpha) cvtColor(tmp, dst, COLOR_mRGBA2RGBA);
        else tmp.copyTo(dst);
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        LOGI("bitmap2Map:ANDROID_BITMAP_FORMAT_RGB_565");
        Mat tmp(info.height, info.width, CV_8UC2, pixels);
        cvtColor(tmp, dst, COLOR_mRGBA2RGBA);
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}


void mat2Bitmap(JNIEnv *env, Mat mat, jobject bitmap, bool needPremultiplyAlpha) {
    AndroidBitmapInfo info;
    void *pixels = 0;
    if (AndroidBitmap_getInfo(env, bitmap, &info) < 0)
        return;
    if (!(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
          info.format == ANDROID_BITMAP_FORMAT_RGB_565))
        return;
    if (!(mat.dims == 2 && info.height == (uint32_t) mat.rows &&
          info.width == (uint32_t) mat.cols)) {
        return;
    }
    if (!(mat.type() == CV_8UC1 || mat.type() == CV_8UC3 || mat.type() == CV_8UC4))
        return;
    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) < 0)
        return;
    if (!pixels)
        return;

    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if (mat.type() == CV_8UC1) {
            LOGI("mat2Bitmap:ANDROID_BITMAP_FORMAT_RGBA_8888:CV_8UC1");
            cvtColor(mat, tmp, COLOR_GRAY2RGBA);
        } else if (mat.type() == CV_8UC3) {
            LOGI("mat2Bitmap:ANDROID_BITMAP_FORMAT_RGBA_8888:CV_8UC3");
            cvtColor(mat, tmp, COLOR_RGB2RGBA);
        } else if (mat.type() == CV_8UC4) {
            LOGI("mat2Bitmap:ANDROID_BITMAP_FORMAT_RGBA_8888:CV_8UC4");
            if (needPremultiplyAlpha) cvtColor(mat, tmp, COLOR_mRGBA2RGBA);
            else mat.copyTo(tmp);
        }
    } else {
        Mat tmp(info.height, info.width, CV_8UC4, pixels);
        if (mat.type() == CV_8UC1) {
            LOGI("mat2Bitmap:ANDROID_BITMAP_FORMAT_RGB_565:CV_8UC1");
            cvtColor(mat, tmp, COLOR_GRAY2BGR565);
        } else if (mat.type() == CV_8UC3) {
            LOGI("mat2Bitmap:ANDROID_BITMAP_FORMAT_RGB_565:CV_8UC3");
            cvtColor(mat, tmp, COLOR_RGB2BGR565);
        } else if (mat.type() == CV_8UC4) {
            LOGI("mat2Bitmap:ANDROID_BITMAP_FORMAT_RGB_565:CV_8UC4");
            cvtColor(mat, tmp, COLOR_RGBA2BGR565);
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);
}


jobject createBitmap(JNIEnv *env, Mat srcData, jobject comfig) {
    jclass java_bitmap_class = (jclass) env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetStaticMethodID(java_bitmap_class, "createBitmap",
                                           "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jobject bitmap = env->CallStaticObjectMethod(java_bitmap_class, mid, srcData.cols, srcData.rows,
                                                 comfig);
    mat2Bitmap(env, srcData, bitmap, 0);
    return bitmap;
}

















