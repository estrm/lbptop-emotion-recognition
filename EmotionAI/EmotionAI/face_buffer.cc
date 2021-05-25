#include "face_buffer.h"

#include <QString>

#include "global.h"

FaceBuffer::FaceBuffer() {}

FaceBuffer::~FaceBuffer() {}

/*
@初始化FaceBuffer类.
*/
void FaceBuffer::Init()
{
    frames_.clear();

    Mat f(xSize, ySize, CV_8UC1, Scalar(0));
    for (unsigned int i = 0;i < BUFFER_MAX_SIZE;++i)
    {
        frames_.push_back(f);
    }
    buffer_size_ = BUFFER_MAX_SIZE;
}

/*
@将最新的人脸图像帧入栈人脸图像缓冲区.
@入参: frame, 最新的一帧人脸图像.
@出参: frames_, 人脸图像缓冲队列.
@返回值: 无.
*/
void FaceBuffer::push_back(Mat &frame)
{
    frames_.erase(frames_.begin());
    frames_.push_back(frame);
}

/*
@获取人脸图像缓冲队列.
*/
vector<Mat> FaceBuffer::peep() const
{
    /*@TEST@
    for (unsigned int i = 0;i < buffer_size_;++i)
    {
        string name = QString::number(i).toStdString();
        imshow(name, frames_.at(i));
    }
    */
    return frames_;
}

/*
@释放缓冲区.
*/
void FaceBuffer::clear()
{
    frames_.clear();
}