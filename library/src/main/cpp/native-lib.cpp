#include <jni.h>
#include <string>
#include "utils.h"
#include <opencv2/opencv.hpp>

#define DEFALUT_CARD_WIDTH 640
#define DEFALUT_CARD_HEIGHT 400
#define FIX_IDCARD_SZIE Size(DEFALUT_CARD_WIDTH,DEFALUT_CARD_HEIGHT)
using namespace std;
using namespace cv;



extern "C"
JNIEXPORT jobject JNICALL
Java_com_eli_find_FindInIDCardUtil_findIdNumber(JNIEnv *env, jclass type, jobject bitmap,
                                                jobject argb8888) {
    //将bitmap转成Mat
    Mat src_img;
    Mat dst_img;
    bitmap2Mat(env, bitmap, &src_img);
    //归一化
    Mat dst;
    resize(src_img, dst, FIX_IDCARD_SZIE);
    //灰度化处理
    cvtColor(src_img, dst, COLOR_RGB2GRAY);
    //二值化
    threshold(dst, dst, 150, 255, THRESH_BINARY);
    //轮廓检测
    vector<vector<Point>> contours;
    vector<Rect> rects;
    //膨胀
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(40, 10));
    erode(dst, dst, erodeElement);
    findContours(dst, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    for (int i = 0; i < contours.size(); ++i) {
        //绘制区域
        Rect rect = boundingRect(contours.at(i));
        //绘制
        rectangle(dst, rect, Scalar(0, 0, 255));
        //遍历所有区域找出符合号码的区域
        //身份证啊后面是有固定宽高比例 大于1:12 小于1:16
        if (rect.width > rect.height * 12 && rect.width < rect.height * 16) {
            rects.push_back(rect);
        }
    }
    //查找坐标最低的
    int lowPoint = 0;
    Rect finalRect;
    for (int i = 0; i < rects.size(); ++i) {
        Rect rect = rects.at(i);
        Point point = rect.tl();
        if (point.y > lowPoint) {
            lowPoint = point.y;
            finalRect = rect;
        }
    }

    dst_img = src_img(finalRect);


    //二值化 增加识别度
    threshold(dst_img, dst_img, 120, 255, THRESH_BINARY);


    //将Mat转为bitmap
//    return createBitmap(env, dst, argb8888);//原图
    return createBitmap(env, dst_img, argb8888);

}