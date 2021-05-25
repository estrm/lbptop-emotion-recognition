/*
@Tite  : Elderly Person’s Emotional Status Monitoring System.
@Author: Sun Yiming
@Email : zscandyz(you know)gmail(dot)com
@Date  : 20180318-Now
*/
#include <QtWidgets/QApplication>

#include "ec_global.h"
#include "emotion_ai_main_window.h"
#include "global.h"

FaceBuffer fb;
FaceTracker ft;
LbpTop lt;
Music mu;
EmotionClassification ec;

/*
@LBP-TOP: g_P, 采样数量; g_R, 采样半径.
@默认采用8邻域
*/
int g_Pxy = kDefaultPxy;
int g_Pxt = kDefaultPxt;
int g_Pyt = kDefaultPyt;
int g_Rx = kDefaultRx;
int g_Ry = kDefaultRy;
int g_Rt = kDefaultRt;

int g_save_image_time_interval = SAVE_IMAGE_TIME_INTERVAL;
string g_save_image_path = SAVE_IMAGE_PATH;
string g_snip_image_path = SNIP_IMAGE_PATH;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    fb.Init();
    ft.Init();
    lt.Init();
    mu.Init();
    ec.Init();

    EmotionAiMainWindow w;
    w.show();
    return app.exec();
}