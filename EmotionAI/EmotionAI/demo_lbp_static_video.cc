#include "demo_lbp_static_video.h"

#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QProgressDialog>

#include "ec_global.h"
#include "global.h"

DemoLBPStaticVideo::DemoLBPStaticVideo(QWidget *parent)
    : QDialog(parent),
    dsv_ui_(new Ui::DemoLBPStaticVideo)
{
    dsv_ui_->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint |
                         Qt::WindowMaximizeButtonHint |
                         Qt::WindowCloseButtonHint);
    InitVideoInfo();
    InitEmotionNum();

    expression_label_num_timer_ = new QTimer();
    expression_label_num_timer_->start(1000);
    connect(dsv_ui_->loadVideoBtn, SIGNAL(clicked()), this, SLOT(OnLoadVideoButtonClicked()));
    connect(dsv_ui_->playVideoBtn, SIGNAL(clicked()), this, SLOT(OnPlayVideoButtonClicked()));
    connect(dsv_ui_->exitDemoBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(expression_label_num_timer_, SIGNAL(timeout()), this, SLOT(UpdateEmotionLabel()));
}

DemoLBPStaticVideo::~DemoLBPStaticVideo()
{
    delete expression_label_num_timer_;
    delete dsv_ui_;
}

/*
@加载视频文件路径.
@入参: 无.
@出参: video_file_path_, 文件路径.
@返回值: 无.
*/
void DemoLBPStaticVideo::OnLoadVideoButtonClicked()
{
    video_file_path_ = QFileDialog::getOpenFileName(this,
                tr("Select Video"),
                QString::fromStdString(TEST_VIDEO_PATH),
                tr("Video File(*.avi *.wmv *.flv *.mp4 *.MOV *.webm *.mpg)"));

    if (video_file_path_.isEmpty())
    {
        return;
    }
    InitEmotionNum();

    /*显示第一帧.*/
    Mat head_frame_;
    video_cam_.open(video_file_path_.toLocal8Bit().toStdString());
    if (!video_cam_.isOpened())
    {
        return;
    }
    is_valid_video_file_path_ = true;
    video_cam_ >> head_frame_;
    if (head_frame_.empty())
    {
        return;
    }
    GetVideoInfo();
    ShowVideoInfo();
    video_cam_.release();

    QImage qimg = QImage((const unsigned char*)head_frame_.data, head_frame_.cols,
                                               head_frame_.rows, head_frame_.step,
                                               QImage::Format_RGB888).rgbSwapped();
    dsv_ui_->staticVideoLabel->setPixmap(QPixmap::fromImage(qimg));
}

/*
@播放视频, 并进行表情识别.
@入参: 无.
@出参: 无.
@返回值: 无.
*/
void DemoLBPStaticVideo::OnPlayVideoButtonClicked()
{
    if (false == is_valid_video_file_path_)
    {
        return;
    }

    video_cam_.open(video_file_path_.toLocal8Bit().toStdString());
    if (!video_cam_.isOpened())
    {
        return;
    }

    /*处理进度条.*/
    QProgressDialog dialogDemo(tr("{ Running, have a coffee. }"),
                               tr("cancel"), 0, video_info_.tatal_frame, this);
    dialogDemo.setWindowTitle(tr("Video Progress"));
    dialogDemo.setWindowModality(Qt::WindowModal);
    dialogDemo.setMinimumSize(300, 100);
    dialogDemo.show();

    /*count, 记录已经处理的图像帧数量.*/
    int count = 0;
    Mat new_frame;
    EmotionType et;
    QImage fimg;
    while (true)
    {
        video_cam_ >> new_frame;
        if (new_frame.empty())
        {
            break;
        }
        input_frame_ = new_frame.clone();
        ec.GetFaceEmotion(new_frame, input_frame_, et);
        CountEmotionNum(et);

        fimg = QImage((const unsigned char*)input_frame_.data, input_frame_.cols,
                                                   input_frame_.rows,input_frame_.step,
                                                   QImage::Format_RGB888).rgbSwapped();
        dsv_ui_->staticVideoLabel->clear();
        dsv_ui_->staticVideoLabel->setPixmap(QPixmap::fromImage(fimg));

        dialogDemo.setValue(count++);
        QCoreApplication::processEvents();
        if (dialogDemo.wasCanceled())
        {
            break;
        }
    }
    is_valid_video_file_path_ = false;
    video_cam_.release();
}

/*
@初始化视频信息.
*/
void DemoLBPStaticVideo::InitVideoInfo()
{
    video_info_.rate = 0;
    video_info_.tatal_frame = 0;
    video_info_.frame_width = 0;
    video_info_.frame_height = 0;
    is_valid_video_file_path_ = false;
}

/*
@获取视频信息.
*/
void DemoLBPStaticVideo::GetVideoInfo()
{
    video_info_.rate = video_cam_.get(CV_CAP_PROP_FPS);
    video_info_.tatal_frame = static_cast<long>(video_cam_.get(CV_CAP_PROP_FRAME_COUNT));
    video_info_.frame_width = video_cam_.get(CV_CAP_PROP_FRAME_WIDTH);
    video_info_.frame_height = video_cam_.get(CV_CAP_PROP_FRAME_HEIGHT);
}

/*
@展示视频信息.
@包括: 视频路径, 总帧数, 帧率, 高, 宽.
*/
void DemoLBPStaticVideo::ShowVideoInfo()
{
    dsv_ui_->label_vpath->setText(video_file_path_);
    dsv_ui_->label_vnum->setText(QString::number(video_info_.tatal_frame));
    dsv_ui_->label_vrate->setText(QString::number(video_info_.rate));
    dsv_ui_->label_vheight->setText(QString::number(video_info_.frame_height));
    dsv_ui_->label_vwidth->setText(QString::number(video_info_.frame_width));
 }

/*
@初始化各种表情数量.
*/
void DemoLBPStaticVideo::InitEmotionNum()
{
    emotion_num_.anger_num = 0;
    emotion_num_.disgust_num = 0;
    emotion_num_.fear_num = 0;
    emotion_num_.happy_num = 0;
    emotion_num_.neutral_num = 0;
    emotion_num_.sadness_num = 0;
    emotion_num_.surprise_num = 0;
}

/*
@计算不同表情的数量.
*/
void DemoLBPStaticVideo::CountEmotionNum(const EmotionType em)
{
    switch (em)
    {
    case ANGER:
        emotion_num_.anger_num++;
        break;
    case DISGUST:
        emotion_num_.disgust_num++;
        break;
    case FEAR:
        emotion_num_.fear_num++;
        break;
    case HAPPY:
        emotion_num_.happy_num++;
        break;
    case NEUTRAL:
        emotion_num_.neutral_num++;
        break;
    case SADNESS:
        emotion_num_.sadness_num++;
        break;
    case SURPRISE:
        emotion_num_.surprise_num++;
        break;
    default:
        break;
    }
}

/*
@更新表情标签数字.
@入参: 无.
@出参: 无.
@返回值: 无.
*/
void DemoLBPStaticVideo::UpdateEmotionLabel()
{
    dsv_ui_->label_an->setText(QString::number(emotion_num_.anger_num));
    dsv_ui_->label_di->setText(QString::number(emotion_num_.disgust_num));
    dsv_ui_->label_fe->setText(QString::number(emotion_num_.fear_num));
    dsv_ui_->label_ha->setText(QString::number(emotion_num_.happy_num));
    dsv_ui_->label_ne->setText(QString::number(emotion_num_.neutral_num));
    dsv_ui_->label_sa->setText(QString::number(emotion_num_.sadness_num));
    dsv_ui_->label_su->setText(QString::number(emotion_num_.surprise_num));
}