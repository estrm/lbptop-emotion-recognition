/*
@系统主界面.
*/
#ifndef EMOTION_H
#define EMOTION_H
#include "ui_emotion_ai_main_window.h"

#include <QDir>
#include <QFileInfoList>
#include <QGraphicsScene>
#include <QLabel>
#include <QTimer>
#include <QtWidgets/QMainWindow>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "demo_lbp_static_image.h"
#include "demo_lbp_static_video.h"
#include "lbptopsettings.h"
#include "storage_settings.h"

using namespace cv;

namespace Ui {
    class EmotionAiMainWindow;
};

class EmotionAiMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EmotionAiMainWindow(QWidget *parent = Q_NULLPTR);
    virtual ~EmotionAiMainWindow();

private slots:
    void OnActionDemoImageTriggered();      /*显示静态图片演示窗口.*/
    void OnActionDemoVideoTriggered();      /*显示静态视频演示窗口.*/
    void OnActionLbptopSettingsTriggered(); /*LBP-TOP参数设置窗口.*/
    void OnActionStorageSettingsTriggered();/*存储路径及存储时间间隔设置.*/
    void OnActionViewLbptopHistTriggered(); /*查看柱状图.*/
    void OnActionAboutEmotionAiTriggered(); /*关于系统.*/

    void OnOpenCamera();                    /*打开摄像头.*/
    void GetCameraFrame();                  /*捕获摄像头帧.*/
    void OnOpenVideo();                     /*打开视频文件.*/
    void GetVideoFrame();                   /*捕获视频帧.*/
    void OnOpenImageSeq();                  /*打开图像序列.*/
    void GetImageSeqFrame();                /*捕获图像序列帧.*/

    void OnPlayPauseButtonPressed();        /*[播放/暂停]按钮.*/
    void OnStopReplayButtonPressed();       /*[停止/重播]按钮.*/
    void OnSnipFrameButtonClicked();        /*抓图.*/

    void DelayChanged();                    /*时延的值被更改.*/
    void SaveEmotionFrame();                /*定时存储表情帧.*/

    void OpenExtension();                   /*打开绘图展示区域.*/
    void CloseExtension();                  /*关闭绘图展示区域.*/

private:
    void ShowBufferFrames(const std::vector<Mat> &volume);  /*展示图像缓冲区内容.*/
    void ShowLbpTopFrames();                                /*展示LBP-TOP合成图像.*/
    void RelaxMusic(const int emotion_label);               /*音乐.*/

    Ui::EmotionAI *ui_;
    DemoLBPStaticImage *demo_static_image_window_;
    DemoLBPStaticVideo *demo_static_video_window_;
    LbpTopSettings *lbptop_settings_window_;
    StorageSettings *storage_settings_window_;

    bool paused_status_;                    /*[暂停/播放]按钮状态.*/
    bool stop_replay_status_;               /*[停止/重播]按钮状态.*/

    VideoCapture camera_cap_;               /*相机实例.*/
    VideoCapture video_cap_;                /*视频实例.*/
    Mat frame_;                             /*保存摄像头抓取的帧.*/
    Mat face_;                              /*从视频帧中提取的人脸图像.*/
    Mat result_frame_;                      /*表情分类完成的图像.*/
    Mat sample_feature_data_;               /*样本特征数据.*/
    Mat xy_;
    Mat xt_;
    Mat yt_;
    Mat hist_image_;

    QTimer *camera_timer_;                  /*相机定时器.*/
    QTimer *video_timer_;                   /*视频定时器.*/
    QTimer *image_timer_;                   /*图像序列定时器.*/
    QTimer *save_image_timer_;              /*表情图像定时存储.*/

    QImage q_image_;
    QImage frame_xy_image_;
    QImage frame_xt_image_;
    QImage frame_yt_image_;
    QImage q_hist_image_;

    QGraphicsScene *main_view_scene_;
    QGraphicsScene *histogram_view_scene_;
    QString file_path_;
    QString directory_path_;                /*打开的文件夹的路径.*/
    QFileInfoList file_info_list_;          /*directory_path_文件夹下文件信息列表.*/

    int file_index_;                        /*打开的图像序列序号.*/
    int fps_;                               /*相机帧率.*/
    int delay_;                             /*时延.*/
    char media_type_;                       /*媒体类型 
                                                c: camera;
                                                v: video;
                                                i: image sequence.
                                            */
};

#endif /*EMOTION_H*/