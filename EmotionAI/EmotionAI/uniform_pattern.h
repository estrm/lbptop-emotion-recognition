/*
@Uniform Pattern, 原始特征数据降维.
*/
#ifndef UNIFORM_PATTERN_H
#define UNIFORM_PATTERN_H

#include <opencv2/opencv.hpp>

using namespace cv;

class UniformPattern
{
public:
    UniformPattern();
    virtual ~UniformPattern();

    void LoadUniformPattern();      /*加载Uniform Pattern.*/
    int IndexOf(const unsigned V);  /*Uniform Pattern 降维.*/

private:
    Mat uniform_pattern_;           /*等价模式序列, 数据均为等价模式类.*/
    int bits_;                      /*采样点数量.*/
};

#endif /*UNIFORM_PATTERN_H*/