#include "lbp.h"

/*
@获取图像的LBP特征向量.
@入参: src_image, 原始图像.
@出参: dst_lbp_image, LBP特征向量.
@返回值: LBP特征向量.
*/
Mat GetLBP(const Mat& src_image, Mat& dst_lbp_image)
{
    unsigned int imagerow, imagecol = 0;
    imagerow = (unsigned int)src_image.rows;
    imagecol = (unsigned int)src_image.cols;

    dst_lbp_image = Mat::zeros(imagerow - 2, imagecol - 2, CV_8UC1);

    uchar lbp_center = src_image.at<uchar>(0, 0);
    uchar code = 0;
    for (unsigned int i = 1;i < imagerow - 1;++i)
    {
        for (unsigned int j = 1;j < imagecol - 1;++j)
        {
            lbp_center = src_image.at<uchar>(i, j);
            code = 0;
            code |= (src_image.at<uchar>(i - 1, j - 1) >= lbp_center) << 7;
            code |= (src_image.at<uchar>(i - 1, j) >= lbp_center) << 6;
            code |= (src_image.at<uchar>(i - 1, j + 1) >= lbp_center) << 5;
            code |= (src_image.at<uchar>(i, j + 1) >= lbp_center) << 4;
            code |= (src_image.at<uchar>(i + 1, j + 1) >= lbp_center) << 3;
            code |= (src_image.at<uchar>(i + 1, j) >= lbp_center) << 2;
            code |= (src_image.at<uchar>(i + 1, j - 1) >= lbp_center) << 1;
            code |= (src_image.at<uchar>(i, j - 1) >= lbp_center) << 0;
            dst_lbp_image.at<uchar>(i - 1, j - 1) = code;
        }
    }
    return dst_lbp_image;
}