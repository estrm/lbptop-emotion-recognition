#ifndef GLOBAL_H
#define GLOBAL_H

#include "face_buffer.h"
#include "face_tracker.h"
#include "lbptop.h"
#include "music.h"
#include "uniform_pattern.h"


#define CascadeClassifier_Model  "TrainModel\\haarcascade_frontalface_default.xml"
#define Uniform_Pattern_File_Dir "TrainModel\\patterns\\"

/*SAVE 存储图像文件目录.*/
#define PRE_S ""
#define SAVE_IMAGE_TIME_INTERVAL 10000
#define SAVE_IMAGE_PATH PRE_S"EmotionImage"
#define SNIP_IMAGE_PATH PRE_S"EmotionImage\\snip"

/*TEST 测试图像文件目录.*/
#define TEST_VIDEO_PATH PRE_S"TestFile\\video\\"
#define TEST_IMAGE_PATH PRE_S"TestFile\\image\\"

extern FaceTracker ft;
extern FaceBuffer fb;
extern LbpTop lt;
extern Music mu;

extern int g_save_image_time_interval;
extern string g_save_image_path;
extern string g_snip_image_path;

/*
@LBP-TOP.
@g_P, 采样点数量; g_R, 采样半径.
*/
extern int g_Pxy;
extern int g_Pxt;
extern int g_Pyt;
extern int g_Rx;
extern int g_Ry;
extern int g_Rt;

const unsigned int xSize = 280;           /*提取脸部图像后，将图像宽度缩放至xSize.*/
const unsigned int ySize = 280;           /*提取脸部图像后，将图像高度缩放至ySize.*/
const unsigned int xBlocks = 14;          /*脸部图像沿X轴方向划分的块数.*/
const unsigned int yBlocks = 14;          /*脸部图像沿Y轴方向划分的块数.*/
const unsigned int BUFFER_MAX_SIZE = 10;  /*图像缓冲区最多存储的图像数量.*/

/*
@kDefaultP: 默认采样点数量; kDefaultR: 默认采样半径.
*/
const int kDefaultPxy = 8;
const int kDefaultPxt = 8;
const int kDefaultPyt = 8;
const int kDefaultRx = 3;
const int kDefaultRy = 3;
const int kDefaultRt = 3;

/*LBP-TOP表情代码-表情类别转换表.*/
static char* Lbptop_Expression_Hash_Table[9] =
{
    "unknown", "anger", "contempt", "disgust", "fear", "happy", "sadness", "surprise"
};
#endif  /*GLOBAL_H*/