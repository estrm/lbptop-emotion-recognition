#ifndef EC_GLOBAL_H
#define EC_GLOBAL_H
#include "emotion_classification.h"
#include "EmotionClassification\lbp.h"
#include "EmotionClassification\hist.h"
#include "EmotionClassification\svm.h"

#define PER_T "TrainModel\\"
#define Lbp_Train_Model            PER_T"lbp_r32.32.model"
#define Lbptop_Train_Model         PER_T"lbptop_r280.280_b14.14.model"
#define EC_CascadeClassifier_Model PER_T"haarcascade_frontalface_default.xml"

/*检测出的人脸图像归一化后的尺寸.*/
const int SAVE_IMAGE_SIZE_HEIGHT = 32;
const int SAVE_IMAGE_SIZE_WIDTH  = 32;

extern EmotionClassification ec;

#endif /*EC_GLOBAL_H*/