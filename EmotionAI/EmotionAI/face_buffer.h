/*
@人脸图像缓冲区.
*/
#ifndef FACE_BUFFER_H
#define FACE_BUFFER_H
#include <vector>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class FaceBuffer
{
public:
    FaceBuffer();
    virtual ~FaceBuffer();

    void Init();
    void push_back(Mat &frame); /*将最新的人脸图像帧入栈人脸图像缓冲区.*/
    vector<Mat> peep() const;

private:
    void clear();               /*释放人脸图像缓冲区.*/

    int buffer_size_;           /*人脸图像缓冲区大小.*/
    vector<Mat>frames_;         /*人脸图像缓冲区内容.*/
};

#endif /*FACE_BUFFER_H*/