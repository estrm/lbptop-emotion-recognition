/*
@静态视频分析.
@描述: 输入一段视频序列, 分析每一帧中的人脸表情, 统计不同表情的数量.
*/
#ifndef DEMO_LBP_STATIC_VIDEO_H
#define DEMO_LBP_STATIC_VIDEO_H
#include "ui_demo_lbp_static_video.h"

#include <QLabel>
#include <QTimer>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "ec_global.h"

using namespace cv;

/*
@视频文件信息.
*/
struct VideoInfo
{
    double rate;           /*帧率.*/
    long int tatal_frame;  /*总帧数.*/
    int frame_width;       /*帧宽.*/
    int frame_height;      /*帧高.*/
};

/*
@不同表情的数量.
*/
struct EmotionNum
{
    unsigned int anger_num;
    unsigned int disgust_num;
    unsigned int fear_num;
    unsigned int happy_num;
    unsigned int neutral_num;
    unsigned int sadness_num;
    unsigned int surprise_num;
};

namespace Ui
{
    class DemoLBPStaticVideo;
};

class DemoLBPStaticVideo : public QDialog
{
    Q_OBJECT

public:
    explicit DemoLBPStaticVideo(QWidget *parent = Q_NULLPTR);
    virtual ~DemoLBPStaticVideo();

private slots:
    void OnLoadVideoButtonClicked();            /*加载静态视频文件路径.*/
    void OnPlayVideoButtonClicked();            /*开始播放视频，并进行表情识别.*/
    void UpdateEmotionLabel();                  /*更新表情标签数字，展示每一种表情的数量.*/
private:
    void InitVideoInfo();                       /*初始化视频信息.*/
    void GetVideoInfo();                        /*获取视频信息.*/
    void ShowVideoInfo();                       /*展示视频信息.*/

    void InitEmotionNum();                      /*初始化各种表情数量.*/
    void CountEmotionNum(const EmotionType em); /*计算不同表情的数量.*/

    Ui::DemoLBPStaticVideo *dsv_ui_;

    QString video_file_path_;                   /*视频文件路径.*/
    bool is_valid_video_file_path_;             /*视频路径文件名是否有效.*/
    VideoCapture video_cam_;                    /*摄像头实例.*/
    Mat input_frame_;                           /*保存视频的每一帧.*/
    QTimer *expression_label_num_timer_;        /*定时器用于定时更新表情统计数据.*/

    struct VideoInfo video_info_;               /*所加载的视频文件信息.*/
    struct EmotionNum emotion_num_;             /*不同表情的数量.*/
};

#endif /*DEMO_LBP_STATIC_VIDEO_H*/