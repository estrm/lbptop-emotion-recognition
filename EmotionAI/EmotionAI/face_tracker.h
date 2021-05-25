/*
@面部区域追踪类.
*/
#ifndef FACE_TRACKER_H
#define FACE_TRACKER_H
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class FaceTracker
{
public:
    FaceTracker();
    virtual ~FaceTracker();

    void Init();
    Rect FaceDetect(const Mat &frame);   /*获取脸部区域.*/

private:
    CascadeClassifier ccf_;              /*分类器.*/
    Mat frame_gray_;                     /*灰度图像.*/
    vector<cv::Rect> face_regions_;      /*人脸矩形区域向量.*/
};

#endif /*FACE_TRACKER_H*/