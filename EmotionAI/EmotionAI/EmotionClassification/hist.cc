#include "hist.h"

/*
@获取灰度图像直方图分布.
@入参: gray_image, 原始灰度图像.
@出参: hist_image, 图像像素值分布向量.
@返回值: 无.
*/
void GetHistValue(const Mat & gray_image, Mat & hist_image)
{
    if (gray_image.empty())
    {
        return;
    }

    const float range[] = { 0, 256 };
    const float* kHistRange = { range };
    const int kHistSize = 256;
    const bool kUniform = true;
    const bool kAccumulate = false;
    calcHist(&gray_image, 1, 0, Mat(), hist_image, 1, &kHistSize, 
                            &kHistRange, kUniform, kAccumulate);
}