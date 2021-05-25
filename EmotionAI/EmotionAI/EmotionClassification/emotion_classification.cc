#pragma warning(disable : 4996)
#include <iostream>

#include "emotion_classification.h"

#include "ec_global.h"
#include "hist.h"
#include "lbp.h"

EmotionClassification::EmotionClassification()
{
    lbp_emotion_probability_ = new double[8];
    lbptop_emotion_probability_ = new double[8];
    model_lbp_ = new svm_model;
    model_lbptop_ = new svm_model;
    model_lbp_ = NULL;
    model_lbptop_ = NULL;
}

EmotionClassification::~EmotionClassification()
{
    delete[] lbp_emotion_probability_;
    delete[] lbptop_emotion_probability_;
    delete model_lbp_;
    delete model_lbptop_;
}

/*
@初始化, 加载分类器模型.
*/
void EmotionClassification::Init()
{
    ccf_.load(EC_CascadeClassifier_Model);
    model_lbptop_ = svm_load_model(Lbptop_Train_Model);
    model_lbp_ = svm_load_model(Lbp_Train_Model);
}

/*
@获取人脸区域坐标.
@入参: image_gray, 输入的灰度图像; faces, 存储人脸坐标.
@出参: faces, 存储检测到的人脸坐标.
@返回值: 无.
*/
void EmotionClassification::GetFaceArea(const Mat &image_gray, vector<Rect> &faces)
{
    ccf_.detectMultiScale(image_gray, faces, 1.1, 3, 0);
}

/*
@获取人脸图像.
@入参: srcImage, 原始图像; faces: 人脸区域坐标.
@出参: face_image, 人脸图像.
@返回值: 无.
*/
void EmotionClassification::GetFaceImage(const Mat &src_image, const vector<Rect> &faces, Mat &face_image)
{
    size_t size = faces.size();
    face_image = src_image(faces[size-1]);
}

/*
@检测人脸坐标, 表情分类.
@入参: src_image, 原始图像.
@出参: mark_image, 标识人脸区域后的原始图像; et, 表情类别代码.
@返回值: sum, 图像中检测到的人脸图像总数.
*/
int EmotionClassification::GetFaceEmotion(const Mat &src_image, Mat &mark_image, EmotionType &et)
{
    if (src_image.empty())
    {
        return -1;
    }

    vector<Rect> faces;
    Mat gray;
    cvtColor(src_image, gray, CV_BGR2GRAY);
    equalizeHist(gray, gray);
    GetFaceArea(gray, faces);

    Mat lbp_face, lbp_hist;
    double face_emotion = 0;
    for (vector<Rect>::const_iterator iter = faces.begin();iter != faces.end();iter++)
    {
        rectangle(mark_image, *iter, Scalar(0, 0, 255), 1, 0);
        Mat temp(gray, (*iter));
        resize(temp, temp, Size(SAVE_IMAGE_SIZE_HEIGHT + 2, SAVE_IMAGE_SIZE_WIDTH + 2),
               (0, 0), (0, 0), INTER_LINEAR);

        /*lbp_hist, 图像的LBP特征柱状图.*/
        GetLBP(temp, lbp_face);
        GetHistValue(lbp_face, lbp_hist);

        /*SVM分类, 识别表情.*/
        face_emotion = SvmLbpEmotion(lbp_hist);
        et = EmotionType(int(face_emotion - 0));

        /*绘制表情文字.*/
        DrawText(mark_image, et, (*iter));
    }

    int sum = 0;
    sum = faces.size();
    faces.clear();
    return sum;
}

/*
@绘制人脸矩形框.
@入参: faces, 人脸坐标; src_image, 原始图像.
@出参: src_image, 标注人脸框的图像.
@返回值: 无.
*/
void EmotionClassification::DrawFaceRect(const vector<Rect> &faces, Mat &src_image)
{
    for (vector<Rect>::const_iterator iter = faces.begin();iter != faces.end();iter++)
    {
        rectangle(src_image, *iter, Scalar(0, 0, 255), 1, 0);
    }
}

/*
@svm分类器判断, 图像属于哪一类表情.
@入参: mat_feature_vector, 原图像LBP特征向量.
@出参: 无.
@返回值: predict_lable, 表情类别代码.
*/
double EmotionClassification::SvmLbpEmotion(const Mat &mat_feature_vector)
{
    if (mat_feature_vector.empty())
    {
        return 0.0;
    }

    svm_node xnode[257];
    for (unsigned int i = 0;i < 256;++i)
    {
        xnode[i].index = i + 1;
        xnode[i].value = mat_feature_vector.at<float>(i);
    }
    xnode[256].index = -1;

    double predict_lable = 0;
    predict_lable = svm_predict_probability(model_lbp_, xnode, lbp_emotion_probability_);
    return predict_lable;
}

/*
@svm分类器判断, 图像属于哪一类表情（LBP-TOP）.
@入参: mat_feature_vector, 原图像LBP-TOP特征向量.
@出参: 无.
@返回值: predict_lable, 表情类别代码.
*/
double EmotionClassification::SvmLbptopEmotion(const Mat &mat_feature_vector)
{
    if (mat_feature_vector.empty())
    {
        return 0.0;
    }

    int node_size = mat_feature_vector.cols;
    svm_node *xnode = new svm_node[node_size + 1];
    for (unsigned int i = 0;i < node_size;++i)
    {
        xnode[i].index = i + 1;
        xnode[i].value = (*((float*)(mat_feature_vector.data + (i) * sizeof(float))));
    }
    xnode[node_size].index = -1;

    double predict_lable = 0;
    predict_lable = svm_predict(model_lbptop_, xnode);
    delete[] xnode;

    return predict_lable;
}

/*
@在原始图像上, 绘制表情文字说明.
@入参: srcImage, 原始图片; emotion_label, 表情标签; rect, 绘制点坐标.
@出参：无
@返回值：无
@*/
void EmotionClassification::DrawText(Mat &src_image, const EmotionType &emotion_label, Rect rect)
{
    string text_emotion_type = "";
    switch (emotion_label)
    {
    case ANGER: text_emotion_type = "ANGER";
        break;
    case DISGUST: text_emotion_type = "DISGUST";
        break;
    case FEAR: text_emotion_type = "FEAR";
        break;
    case HAPPY: text_emotion_type = "HAPPY";
        break;
    case NEUTRAL: text_emotion_type = "NEUTRAL";
        break;
    case SADNESS: text_emotion_type = "SADNESS";
        break;
    case SURPRISE: text_emotion_type = "SURPRISE";
        break;
    case UNKNOW: text_emotion_type == "UNKNOW";
    default:
        text_emotion_type = "UnKnow";
        break;
    }

    /*获取文本框的长宽.*/
    const int font_face = cv::FONT_HERSHEY_SIMPLEX;
    const double font_scale = 1;
    const int thickness = 1;
    int baseline;
    cv::Size text_size = cv::getTextSize(text_emotion_type, font_face,
                font_scale, thickness, &baseline);
    cv::Point origin;
    origin.x = rect.x;
    origin.y = rect.y;
    cv::putText(src_image, text_emotion_type, origin, font_face, font_scale,
                cv::Scalar(0, 255, 255), thickness, 8, 0);
    
    /*置信度.*/
    string text_emotion_probability = "";
    text_emotion_probability = 
        to_string(lbp_emotion_probability_[emotion_label - 1] * 100) + "%";
    cv::Point origin_num;
    origin_num.x = rect.x;
    origin_num.y = rect.y + text_size.height;
    cv::putText(src_image, text_emotion_probability, origin_num, font_face, 0.5,
        cv::Scalar(255, 255, 255), thickness, 8, 0);
}