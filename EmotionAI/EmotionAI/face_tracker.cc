#include "face_tracker.h"

#include "global.h"

FaceTracker::FaceTracker() {}

FaceTracker::~FaceTracker() {}

/*
@初始化FaceTracker类.
*/
void FaceTracker::Init()
{
    ccf_.load(CascadeClassifier_Model);
}

/*
@获取脸部区域.
@入参: frame, 原始人脸图像.
@出参: 无.
@返回值: roi, 面部区域.
*/
Rect FaceTracker::FaceDetect(const Mat &frame)
{
    if (frame.empty())
    {
        return (Rect(0, 0, 0, 0));
    }
    resize(frame, frame_gray_, Size(frame.cols / 2, frame.rows / 2));
    cvtColor(frame_gray_, frame_gray_, CV_BGR2GRAY);
    equalizeHist(frame_gray_, frame_gray_);
    ccf_.detectMultiScale(frame_gray_, face_regions_, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(30, 30));

    if (face_regions_.empty())
    {
        return Rect(0, 0, 0, 0);
    }

    Rect roi = face_regions_.at(face_regions_.size() - 1);
    roi &= Rect(0, 0, frame_gray_.cols - 1, frame_gray_.rows - 1);
    roi.x <<= 1;
    roi.y <<= 1;
    roi.width <<= 1;
    roi.height <<= 1;
    return roi;
}