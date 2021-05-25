/*
@静态图像分析演示.
@描述：输入一张带有人脸的图像，展示人脸识别的过程.
*/
#ifndef DEMO_LBP_STATIC_IMAGE_H
#define DEMO_LBP_STATIC_IMAGE_H
#include "ui_demo_lbp_static_image.h"

#include <QLabel>
#include <QTimer>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "emotion_classification.h"

/*图像展示Label的尺寸.*/
const int SHOW_IMAGE_LABEL_SIZE = 200;

namespace Ui {
    class DemoLBPStaticImage;
};

class DemoLBPStaticImage : public QDialog
{
    Q_OBJECT

public:
    explicit DemoLBPStaticImage(QWidget *parent = Q_NULLPTR);
    ~DemoLBPStaticImage();

private slots:
    void OnLoadImageButtonClicked();                        /*加载图片文件.*/
    void OnSingleFaceRecognitionButtonClicked();            /*表情识别(单面孔).*/
    void OnMultiFaceRecognitionButtonClicked();             /*表情识别(多面孔).*/

    void ViewOriginalImageClicked();                        /*原图.*/
    void ViewGrayImageClicked();                            /*灰度图.*/
    void ViewGrayEqualImageClicked();                       /*直方图均衡化.*/
    void ViewFaceDetectionImageClicked();                   /*人脸图像.*/
    void ViewFaceCutImageClicked();                         /*人脸区域裁剪.*/
    void ViewNormalImageClicked();                          /*尺寸归一化.*/

private:
    void InitMatImage();                                    /*初始化存储图片的Mat类型变量.*/
    void GetEmotionResult(const EmotionType em);            /*展示表情识别结果.*/
    void CalcScaleNum(const int width, const int height);   /*计算图像的缩放因子.*/

    Ui::DemoLBPStaticImage *dsi_ui_;

    QString image_file_path_;                               /*图片文件路径.*/
    bool is_valid_file_path_;                               /*图片路径文件名是否有效.*/

    Mat image_original_;                                    /*加载的本地图像.*/
    Mat image_gray_;                                        /*灰度图.*/
    Mat image_equalize_hist_;                               /*直方图均衡化.*/
    Mat image_face_detection_;                              /*检测到的人脸区域.*/
    Mat image_face_cut_;                                    /*裁剪人脸区域.*/
    Mat image_normal_;                                      /*人脸区域归一化.*/

    Mat image_multi_face_result_;                           /*多表情识别的结果.*/
    QString expression_recognition_result_;                 /*情绪类型的结果.*/

    int kScaleWidth_;                                       /*展示图像标签，缩放后图像的宽.*/
    int kScaleHeight_;                                      /*展示图像标签，缩放后图像的高.*/
};

#endif /*DEMO_LBP_STATIC_IMAGE_H*/