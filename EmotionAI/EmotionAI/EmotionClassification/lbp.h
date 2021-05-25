#ifndef LBP_H
#define LBP_H
#include "opencv2/opencv.hpp"

using namespace cv;

/*
@获取图像的LBP特征.
*/
Mat GetLBP(const Mat& src_image, Mat& dst_lbp_image);

#endif /*LBP_H*/