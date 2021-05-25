/*
@表情分类模块.
@表情标签与类别对照表.
@LBP      1     2        3       4      5      6       7
          Anger Disgust  Fear    Happy  Neutra Sadness Surprise
@LBP-TOP  1     2        3       4      5      6       7
          Anger Contempt Disgust Fear   Happy  Sadness Surprise
*/
#ifndef EMOTIONCLASSIFICATION_H
#define EMOTIONCLASSIFICATION_H

#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/ml.hpp"
#include "opencv2/opencv.hpp"

#include "EmotionClassification\svm.h"

using namespace cv;
using namespace std;

enum EmotionType
{
    ANGER = 1,
    DISGUST = 2,
    FEAR = 3,
    HAPPY = 4,
    NEUTRAL = 5,
    SADNESS = 6,
    SURPRISE = 7,
    UNKNOW = 8
};

class EmotionClassification
{
public:
    EmotionClassification();
    virtual ~EmotionClassification();

    void Init();

    /*获取人脸区域坐标.*/
    void GetFaceArea(const Mat &image_gray, vector<Rect> &faces);

    /*截取人脸区域图像.*/
    void GetFaceImage(const Mat &src_image, const vector<Rect> &faces, Mat &face_image);
    
    /*获取人脸表情类别.*/
    int GetFaceEmotion(const Mat &src_image, Mat &mark_image, EmotionType &et);
    
    /*绘制人脸矩形框.*/
    void DrawFaceRect(const vector<Rect> &faces, Mat &srcImage);
    
    double SvmLbpEmotion(const Mat &mat_feature_vector);
    double SvmLbptopEmotion(const Mat &mat_feature_vector);

    double *lbp_emotion_probability_;
    double *lbptop_emotion_probability_;

private:
    /*在原始图像上，绘制表情文字说明.*/
    void DrawText(Mat &srcImage, const EmotionType &emotion_label, Rect rect);

    CascadeClassifier ccf_;
    svm_model *model_lbp_;
    svm_model *model_lbptop_;
}; /*EmotionClassification*/

#endif /*EMOTIONCLASSIFICATION_H*/