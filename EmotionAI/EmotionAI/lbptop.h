/*
@计算图像序列的LBP-TOP特征.
*/
#ifndef LBPTOP_H
#define LBPTOP_H

#include <vector>

#include <opencv2/opencv.hpp>

#include "uniform_pattern.h"

using namespace cv;
using namespace std;

class LbpTop
{
public:
    LbpTop();
    virtual ~LbpTop();

    void Init();

    Mat GenerateSampleData(const vector<Mat> &volume);  /*提取样本LBP-TOP特征.*/
    Mat GetHistImage(const cv::Mat &hist);              /*放大Hist图数值, 以可视化.*/
    void GetFullFrame(Mat &xy, Mat &xt, Mat &yt) const; /*获取完整的LBP-TOP帧图像.*/

private:
    Mat GetLBPtoHist(Mat &frame, const int P, const int r1, const int r2);
                                                        /*将LBP-TOP特征图转化为Hist柱状图.*/
    void NormalizeHist(Mat &matG);                      /*柱状图数据规范化，[0, 1)空间.*/

    UniformPattern p_;
    Point *nbr_table_;
    Mat full_frame_xy;                                  /*X-Y维平面特征图像.*/
    Mat full_frame_xt;                                  /*X-T维平面特征图像.*/
    Mat full_frame_yt;                                  /*Y-T维平面特征图像.*/
};

#endif /*LBPTOP_H*/