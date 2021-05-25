#include "emotion_ai_main_window.h"

#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter.h>
#include <QThread>
#include <QTreeView>

#include "ec_global.h"
#include "global.h"

EmotionAiMainWindow::EmotionAiMainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui_(new Ui::EmotionAI)
{
    ui_->setupUi(this);
    demo_static_image_window_ = new DemoLBPStaticImage();
    demo_static_video_window_ = new DemoLBPStaticVideo();
    lbptop_settings_window_ = new LbpTopSettings(this);
    storage_settings_window_ = new StorageSettings(this);

    main_view_scene_ = new QGraphicsScene;
    ui_->mainView->setScene(main_view_scene_);

    this->resize(700, 683);
    ui_->label_smile->setPixmap(QPixmap::fromImage(QImage(":/EmotionAI/Resources/boy2.png").
        scaled(51, 51, Qt::KeepAspectRatio)));
    ui_->closeDrawerBtn->hide();
    ui_->playPauseBtn->setDisabled(true);
    ui_->stopReplayBtn->setDisabled(true);
    ui_->delaySpinBox->setRange(0, 10000);
    ui_->delayLabel->setBuddy(ui_->delaySpinBox);
    ui_->delaySpinBox->setDisabled(true);
    ui_->statusBar->showMessage("No media.");

    paused_status_ = false;
    stop_replay_status_ = true;

    camera_timer_ = new QTimer(this);
    video_timer_ = new QTimer(this);
    image_timer_ = new QTimer(this);
    save_image_timer_ = new QTimer(this);

    /*定时保存图像.*/
    save_image_timer_->setInterval(g_save_image_time_interval);
    save_image_timer_->start();
    connect(save_image_timer_, SIGNAL(timeout()), this, SLOT(SaveEmotionFrame()));

    /*Menu Bar.*/
    /*menu_1 文件.*/
    connect(ui_->actionOpen_Camera, SIGNAL(triggered()), this, SLOT(OnOpenCamera()));
    connect(camera_timer_, SIGNAL(timeout()), this, SLOT(GetCameraFrame()));
    connect(ui_->actionOpen_Video_File, SIGNAL(triggered()), this, SLOT(OnOpenVideo()));
    connect(video_timer_, SIGNAL(timeout()), this, SLOT(GetVideoFrame()));
    connect(ui_->actionOpen_Image_Seq, SIGNAL(triggered()), this, SLOT(OnOpenImageSeq()));
    connect(image_timer_, SIGNAL(timeout()), this, SLOT(GetImageSeqFrame()));
    connect(ui_->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    /*menu_2 演示.*/
    connect(ui_->actionDemoImage, SIGNAL(triggered()), this, SLOT(OnActionDemoImageTriggered()));
    connect(ui_->actionDemoVideo, SIGNAL(triggered()), this, SLOT(OnActionDemoVideoTriggered()));

    /*menu_3 视图.*/
    connect(ui_->actionViewLbptopHist, SIGNAL(triggered()), this, SLOT(OnActionViewLbptopHistTriggered()));

    /*menu_4 设置.*/
    connect(ui_->actionLBP_TOP, SIGNAL(triggered()), this, SLOT(OnActionLbptopSettingsTriggered()));
    connect(ui_->actionStorage, SIGNAL(triggered()), this, SLOT(OnActionStorageSettingsTriggered()));
    connect(ui_->action_EmotionAI, SIGNAL(triggered()), this, SLOT(OnActionAboutEmotionAiTriggered()));

    /*Tool Bar.*/
    /*File Bar.*/
    connect(ui_->action_open_camera, SIGNAL(triggered()), this, SLOT(OnOpenCamera()));
    connect(ui_->action_open_video_file, SIGNAL(triggered()), this, SLOT(OnOpenVideo()));
    connect(ui_->action_open_image_seq, SIGNAL(triggered()), this, SLOT(OnOpenImageSeq()));

    /*Demo Bar incling image and video.*/
    connect(ui_->action_demo_image, SIGNAL(triggered()), this, SLOT(OnActionDemoImageTriggered()));
    connect(ui_->action_demo_video, SIGNAL(triggered()), this, SLOT(OnActionDemoVideoTriggered()));

    /*界面美化.*/
    connect(ui_->openDrawerBtn, SIGNAL(clicked()), this, SLOT(OpenExtension()));
    connect(ui_->closeDrawerBtn, SIGNAL(clicked()), this, SLOT(CloseExtension()));

    /*控制按钮.*/
    connect(ui_->playPauseBtn, SIGNAL(pressed()), this, SLOT(OnPlayPauseButtonPressed()));
    connect(ui_->stopReplayBtn, SIGNAL(pressed()), this, SLOT(OnStopReplayButtonPressed()));
    connect(ui_->CatchCameraBtn, SIGNAL(pressed()), this, SLOT(OnSnipFrameButtonClicked()));
    connect(ui_->delaySpinBox, SIGNAL(valueChanged(int)), this, SLOT(DelayChanged()));
}

EmotionAiMainWindow::~EmotionAiMainWindow()
{
    delete camera_timer_;
    delete video_timer_;
    delete image_timer_;
    delete save_image_timer_;

    delete demo_static_image_window_;
    delete demo_static_video_window_;
    delete lbptop_settings_window_;
    delete storage_settings_window_;
    delete ui_;
}

/*
@演示1, LBP静态图片表情识别过程.
*/
void EmotionAiMainWindow::OnActionDemoImageTriggered()
{
    demo_static_image_window_->show();
}

/*
@演示2, LBP静态视频表情识别.
*/
void EmotionAiMainWindow::OnActionDemoVideoTriggered()
{
    demo_static_video_window_->show();
}

/*
@LBP-TOP参数设定.
@P, 邻域数量; R, 邻域搜索半径.
*/
void EmotionAiMainWindow::OnActionLbptopSettingsTriggered()
{
    lbptop_settings_window_->show();
}

/*
@存储路径及存储时间间隔设置.
*/
void EmotionAiMainWindow::OnActionStorageSettingsTriggered()
{
    storage_settings_window_->show();
}

/*
@关于系统.
*/
void EmotionAiMainWindow::OnActionAboutEmotionAiTriggered()
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("About Emo - Emotion AI"));
    msgBox.setText(tr("Emotion AI"));
    msgBox.setInformativeText(tr("Version 1.0.0\n"
                                 "Emo is a system of expression recognition.\n\n"
                                 "Copyright (C) 2018 The Anaian Inc."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
}

/*
@查看特征柱状图.
*/
void EmotionAiMainWindow::OnActionViewLbptopHistTriggered()
{
    if (hist_image_.empty())
    {
        QMessageBox::warning(this, "warning", "No Feature Hist",
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    /*CV_GUI_EXPANDED.*/
    namedWindow("histogram of Feature Image", CV_GUI_EXPANDED);
    imshow("histogram of Feature Image", hist_image_);
}

/*
@打开摄像头.
*/
void EmotionAiMainWindow::OnOpenCamera()
{
    ui_->statusBar->showMessage("Camera is opening.");
    if (camera_cap_.isOpened())
    {
        camera_timer_->stop();
        save_image_timer_->stop();
        camera_cap_.release();
    }
    if (video_cap_.isOpened())
    {
        video_timer_->stop();
        save_image_timer_->stop();
        video_cap_.release();
    }
    camera_cap_.open(0);
    delay_ = 40;     /*默认时延.*/

    int cols = camera_cap_.get(CV_CAP_PROP_FRAME_WIDTH);
    int rows = camera_cap_.get(CV_CAP_PROP_FRAME_HEIGHT);
    main_view_scene_->setSceneRect(0, 0, cols, rows);
    ui_->playPauseBtn->setEnabled(true);
    ui_->playPauseBtn->setText(tr("Pause"));
    ui_->stopReplayBtn->setEnabled(true);
    ui_->delaySpinBox->setValue(delay_);
    ui_->delaySpinBox->setEnabled(true);
    paused_status_ = false;
    stop_replay_status_ = true;
    media_type_ = 'c';

    ui_->statusBar->showMessage(tr("Camera Opened."));
    ui_->stopReplayBtn->setText(tr("Stop"));
    camera_timer_->setInterval(delay_);
    camera_timer_->start();
    save_image_timer_->setInterval(g_save_image_time_interval);
    save_image_timer_->start();
}

/*
@捕获摄像头帧信息.
*/
void EmotionAiMainWindow::GetCameraFrame()
{
    camera_cap_ >> frame_;
    if (frame_.empty())
    {
        return;
    }

    /*获取感兴趣区域, 即人脸区域.*/
    Rect roi = ft.FaceDetect(frame_);
    if (roi.area() > 0)
    {
        frame_(roi).copyTo(face_);
        cv::resize(face_, face_, Size(xSize, ySize));
        cvtColor(face_, face_, CV_BGR2GRAY);
        fb.push_back(face_);
        cv::rectangle(frame_, roi, cv::Scalar(0, 0, 255), 2);

        vector<Mat> volume = fb.peep();

        /*计算样本特征数据，即LBP-TOP特征.*/
        sample_feature_data_ = lt.GenerateSampleData(volume);

        ShowBufferFrames(volume);
        lt.GetFullFrame(xy_, xt_, yt_);
        ShowLbpTopFrames();
    }
    q_image_ = QImage((const unsigned char*)frame_.data, frame_.cols,
                                            frame_.rows,frame_.step,
                                            QImage::Format_RGB888).rgbSwapped();
    main_view_scene_->clear();
    main_view_scene_->addPixmap(QPixmap::fromImage(q_image_));
    ui_->statusBar->showMessage(tr("Camera is running."));
}

/*
@打开视频文件.
*/
void EmotionAiMainWindow::OnOpenVideo()
{
    file_path_ = QFileDialog::getOpenFileName(this,
        tr("Select Video"),
        QString::fromStdString(TEST_VIDEO_PATH),
        tr("Video File(*.avi *.wmv *.flv *.mp4 *.MOV *.webm *.mpg)"));

    if (file_path_.isEmpty())
    {
        return;
    }
    ui_->statusBar->showMessage(tr("Opening the video file."));
    if (camera_cap_.isOpened())
    {
        camera_timer_->stop();
        save_image_timer_->stop();
        camera_cap_.release();
    }
    if (video_cap_.isOpened())
    {
        video_timer_->stop();
        save_image_timer_->stop();
        video_cap_.release();
    }

    video_cap_.open(file_path_.toLocal8Bit().toStdString());
    fps_ = video_cap_.get(CV_CAP_PROP_FPS);
    delay_ = int(1000 * 1.0 / fps_);

    ui_->playPauseBtn->setEnabled(true);
    ui_->playPauseBtn->setText(tr("Pause"));
    ui_->stopReplayBtn->setEnabled(true);
    ui_->delaySpinBox->setValue(delay_);
    ui_->delaySpinBox->setEnabled(true);

    int cols = video_cap_.get(CV_CAP_PROP_FRAME_WIDTH);
    int rows = video_cap_.get(CV_CAP_PROP_FRAME_HEIGHT);
    main_view_scene_->setSceneRect(0, 0, cols, rows);
    paused_status_ = false;
    stop_replay_status_ = true;
    media_type_ = 'v';

    ui_->statusBar->showMessage(tr("The video file Opened."));
    ui_->stopReplayBtn->setText(tr("Stop"));

    video_timer_->setInterval(delay_);
    video_timer_->start();
    save_image_timer_->setInterval(g_save_image_time_interval);
    save_image_timer_->start();
}

/*
@捕获视频帧.
*/
void EmotionAiMainWindow::GetVideoFrame()
{
    video_cap_ >> frame_;
    if (frame_.empty())
    {
        video_timer_->stop();
        save_image_timer_->stop();
        OnStopReplayButtonPressed();
        return;
    }

    /*表情识别模块.*/
    Rect roi = ft.FaceDetect(frame_);
    if (roi.area() > 0)
    {
        frame_(roi).copyTo(face_);
        cv::resize(face_, face_, Size(xSize, ySize));
        cvtColor(face_, face_, CV_BGR2GRAY);
        fb.push_back(face_);
        cv::rectangle(frame_, roi, cv::Scalar(0, 0, 255), 2);

        vector<Mat> volume = fb.peep();

        /*计算样本特征, 即LBP-TOP数据.*/
        sample_feature_data_ = lt.GenerateSampleData(volume);

        ShowBufferFrames(volume);
        lt.GetFullFrame(xy_, xt_, yt_);
        ShowLbpTopFrames();
    }

    q_image_ = QImage((const uchar*)frame_.data, frame_.cols, frame_.rows,
        frame_.step, QImage::Format_RGB888).rgbSwapped();
    main_view_scene_->clear();
    main_view_scene_->addPixmap(QPixmap::fromImage(q_image_));
    ui_->statusBar->showMessage(tr("Video is playing."));
}

/*
@打开图像序列.
*/
void EmotionAiMainWindow::OnOpenImageSeq()
{
    QFileDialog *fd = new QFileDialog;
    QTreeView *tree = fd->findChild<QTreeView*>();
    fd->setFileMode(QFileDialog::Directory);
    fd->setWindowTitle(tr("Select Image Sequence."));
    fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);

    int result = fd->exec();
    if (!result)  /*result = 0*/
    {
        return;
    }
    directory_path_ = fd->selectedFiles()[0];
    QDir dir(directory_path_);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.bmp";
    file_info_list_ = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

    if (file_info_list_.empty())
    {
        QMessageBox::critical(this, "Error", "No Image File Found");
        return;
    }
    file_index_ = 0;

    if (camera_cap_.isOpened())
    {
        camera_timer_->stop();
        save_image_timer_->stop();
        camera_cap_.release();
    }
    if (video_cap_.isOpened())
    {
        video_timer_->stop();
        save_image_timer_->stop();
        video_cap_.release();
    }
    /*默认延时.*/
    delay_ = 250;

    ui_->delaySpinBox->setValue(delay_);
    ui_->playPauseBtn->setEnabled(true);
    ui_->playPauseBtn->setText(tr("Pause"));
    ui_->stopReplayBtn->setEnabled(true);
    paused_status_ = false;
    stop_replay_status_ = true;
    media_type_ = 'i';

    ui_->stopReplayBtn->setText(tr("Stop"));

    image_timer_->setInterval(delay_);
    image_timer_->start();

    ui_->statusBar->showMessage(tr("open the image seq."));
}

/*
@捕获图像序列帧
*/
void EmotionAiMainWindow::GetImageSeqFrame()
{
    frame_ = imread(file_info_list_.at(file_index_).absoluteFilePath().toLocal8Bit().toStdString());
    main_view_scene_->setSceneRect(0, 0, frame_.cols, frame_.rows);

    Rect roi = ft.FaceDetect(frame_);
    if (roi.area() > 0)
    {
        frame_(roi).copyTo(face_);

        cv::resize(face_, face_, Size(xSize, ySize));
        cvtColor(face_, face_, CV_BGR2GRAY);
        fb.push_back(face_);
        cv::rectangle(frame_, roi, cv::Scalar(0, 0, 255), 2);
        vector<Mat> volume = fb.peep();

        /*计算样本特征, 即LBP-TOP特征.*/
        sample_feature_data_ = lt.GenerateSampleData(volume);

        ShowBufferFrames(volume);
        lt.GetFullFrame(xy_, xt_, yt_);
        ShowLbpTopFrames();
    }

    q_image_ = QImage((const uchar*)frame_.data, frame_.cols, frame_.rows,
        frame_.step, QImage::Format_RGB888).rgbSwapped();
    main_view_scene_->clear();
    main_view_scene_->addPixmap(QPixmap::fromImage(q_image_));

    /*图像序列加载完成.*/
    if (file_index_ == file_info_list_.size() - 1)
    {
        ui_->statusBar->showMessage(tr("Recognizing expression, please wait."));
        int ret = (int)ec.SvmLbptopEmotion(sample_feature_data_);
        RelaxMusic(ret);

        image_timer_->stop();
        save_image_timer_->stop();
        OnStopReplayButtonPressed();

        QDateTime current_time = QDateTime::currentDateTime();
        QString current_date = current_time.toString("yyyyMMdd hh:mm:ss");
        ui_->label_emo_time->setText(tr(current_date.toLocal8Bit().toStdString().c_str()));
        ui_->label_emo->setText(tr(Lbptop_Expression_Hash_Table[ret]));

        string display_message = "The type of emotion is:\n\n";
        display_message += Lbptop_Expression_Hash_Table[ret];
        QMessageBox::information(this, "Emotion", QString::fromStdString(display_message));
    }
    else
    {
        file_index_++;
        ui_->statusBar->showMessage(tr("Image sequence is playing."));
    }
}

/*
@抓图.
*/
void EmotionAiMainWindow::OnSnipFrameButtonClicked()
{
    if (frame_.empty())
    {
        QMessageBox::warning(this, "warning", "No media",
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    imshow("抓图", frame_);

    /*@TEST@*/
    string path  = g_snip_image_path + "//";
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyyMMddhhmmss");
    path += current_date.toLocal8Bit().toStdString() + "-test.jpg";
    imwrite(path, frame_);
}

/*
@[播放/暂停]按钮触发.
*/
void EmotionAiMainWindow::OnPlayPauseButtonPressed()
{
    paused_status_ = !paused_status_;
    if (paused_status_)
    {
        switch (media_type_)
        {
        case 'c': /*camera.*/
            camera_timer_->stop();
            save_image_timer_->stop();
            break;
        case 'v': /*video.*/
            video_timer_->stop();
            save_image_timer_->stop();
            break;
        case 'i': /*image sequence.*/
            image_timer_->stop();
            save_image_timer_->stop();
            break;
        default:
            break;
        }
        mu.Stop();
        ui_->playPauseBtn->setText(tr("Play"));
        ui_->statusBar->showMessage(tr("Paused"));
    }
    else
    {
        switch (media_type_)
        {
        case 'c':   /*camera.*/
            camera_timer_->start();
            save_image_timer_->start();
            break;
        case 'v':   /*video.*/
            video_timer_->start();
            save_image_timer_->start();
            break;
        case 'i':   /*image sequence.*/
            image_timer_->start();
            break;
        default:
            break;
        }
        ui_->playPauseBtn->setText(tr("Pause"));
        ui_->statusBar->showMessage(tr("Media is playing."));
    }
}

/*
@[停止/重播]按钮触发.
*/
void EmotionAiMainWindow::OnStopReplayButtonPressed()
{
    stop_replay_status_ = !stop_replay_status_;
    if (stop_replay_status_)
    {
        int cols, rows = 0;
        switch (media_type_)
        {
        case 'v':   /*video.*/
            video_cap_.release();
            video_cap_.open(file_path_.toLocal8Bit().toStdString());
            cols = video_cap_.get(CV_CAP_PROP_FRAME_WIDTH);
            rows = video_cap_.get(CV_CAP_PROP_FRAME_HEIGHT);
            main_view_scene_->setSceneRect(0, 0, cols, rows);
            ui_->stopReplayBtn->setText(tr("Stop"));
            video_timer_->start();
            save_image_timer_->start();
            break;
        case 'i':   /*image_sequence.*/
            file_index_ = 0;
            image_timer_->start();
            ui_->stopReplayBtn->setText(tr("Stop"));
            break;
        default:
            break;
        }
        paused_status_ = false;
        ui_->playPauseBtn->setEnabled(true);
        ui_->playPauseBtn->setText(tr("Pause"));
        ui_->statusBar->showMessage(tr("Replay"));
    }
    else
    {
        if (camera_cap_.isOpened())
        {
            save_image_timer_->stop();
            camera_timer_->stop();
            camera_cap_.release();
            ui_->stopReplayBtn->setText(tr("Stop/Replay"));
            ui_->stopReplayBtn->setDisabled(true);
        }
        else if (video_cap_.isOpened())
        {
            save_image_timer_->stop();
            video_timer_->stop();
            video_cap_.release();
            ui_->stopReplayBtn->setText(tr("Replay"));
        }
        else if (media_type_ == 'i')
        {
            save_image_timer_->stop();
            image_timer_->stop();
            ui_->stopReplayBtn->setText(tr("Replay"));
        }
        mu.Stop();
        ui_->playPauseBtn->setText(tr("Play/Pause"));
        ui_->playPauseBtn->setDisabled(true);
        ui_->delaySpinBox->setDisabled(true);
        ui_->statusBar->showMessage(tr("Stopped."));
    }
}

/*
@时延的值被更改.
*/
void EmotionAiMainWindow::DelayChanged()
{
    delay_ = ui_->delaySpinBox->value();
    switch (media_type_)
    {
    case 'c':   /*camera.*/
        camera_timer_->setInterval(delay_);
        break;
    case 'v':   /*video.*/
        video_timer_->setInterval(delay_);
        break;
    default:
        break;
    }
}

/*
@打开绘图展示区域.
*/
void EmotionAiMainWindow::OpenExtension()
{
    ui_->openDrawerBtn->hide();
    ui_->closeDrawerBtn->show();
    resize(1273, 683);
}

/*
@关闭绘图展示区域.
*/
void EmotionAiMainWindow::CloseExtension()
{
    ui_->openDrawerBtn->show();
    ui_->closeDrawerBtn->hide();
    resize(700, 683);
}

/*
@展示缓冲区图像帧.
*/
void EmotionAiMainWindow::ShowBufferFrames(const std::vector<Mat> &volume)
{
    int buffer_size = volume.size();
    int step_len = buffer_size / 5;
    Mat m;
    QImage m_image;

    /*Just show 5 images.*/
    for (unsigned int i = 0;i < 5; ++i)
    {
        volume.at(buffer_size - i * step_len - 1).copyTo(m);
        cv::resize(m, m, Size(100, 100));
        m_image = QImage((const uchar*)m.data, m.cols, m.rows,
                m.step, QImage::Format_Indexed8).rgbSwapped();
        switch (i)
        {
        case 0:
            ui_->buffer5->setPixmap(QPixmap::fromImage(m_image));
            break;
        case 1:
            ui_->buffer4->setPixmap(QPixmap::fromImage(m_image));
            break;
        case 2:
            ui_->buffer3->setPixmap(QPixmap::fromImage(m_image));
            break;
        case 3:
            ui_->buffer2->setPixmap(QPixmap::fromImage(m_image));
            break;
        case 4:
            ui_->buffer1->setPixmap(QPixmap::fromImage(m_image));
            break;
        default:
            break;
        }
    }
}

/*
@展示人脸LBP-TOP特征图像.
*/
void EmotionAiMainWindow::ShowLbpTopFrames()
{
    /*X-Y.*/
    cv::resize(xy_, xy_, cv::Size(150, 150));
    frame_xy_image_ = QImage((const uchar*)xy_.data, xy_.cols, xy_.rows,
                        xy_.step, QImage::Format_Indexed8).rgbSwapped();
    ui_->frameXYView->setPixmap(QPixmap::fromImage(frame_xy_image_));

    /*X-T.*/
    cv::resize(xt_, xt_, cv::Size(150, 150));
    frame_xt_image_ = QImage((const uchar*)xt_.data, xt_.cols, xt_.rows,
                        xt_.step, QImage::Format_Indexed8).rgbSwapped();
    ui_->frameXTView->setPixmap(QPixmap::fromImage(frame_xt_image_));

    /*Y-T.*/
    cv::resize(yt_, yt_, cv::Size(150, 150));
    frame_yt_image_ = QImage((const uchar*)yt_.data, yt_.cols, yt_.rows,
                        yt_.step, QImage::Format_Indexed8).rgbSwapped();
    ui_->frameYTView->setPixmap(QPixmap::fromImage(frame_yt_image_));

    hist_image_ = lt.GetHistImage(sample_feature_data_);
    cv::resize(hist_image_, hist_image_, cv::Size(509, 170));
    q_hist_image_ = QImage((const uchar*)hist_image_.data, hist_image_.cols, 
                                         hist_image_.rows, hist_image_.step,
                                         QImage::Format_Indexed8).rgbSwapped();
    ui_->lbptopView->setPixmap(QPixmap::fromImage(q_hist_image_));
}

/*
@定时存储表情帧
@TODO: 多线程
*/
void EmotionAiMainWindow::SaveEmotionFrame()
{
    Mat will_save_frame = frame_.clone();
    result_frame_ = sample_feature_data_;
    if (will_save_frame.empty() || sample_feature_data_.empty())
    {
        return;
    }
    int ret = (int)ec.SvmLbptopEmotion(result_frame_);
    RelaxMusic(ret);
    /*
        QMessageBox::information(this, "Emotion", Lbptop_Expression_Hash_Table[ret]);
    */
    string txt = Lbptop_Expression_Hash_Table[ret];
    string path = g_save_image_path + "//";
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy.MM.dd-hh.mm.ss-ddd-");

    path += current_date.toLocal8Bit().toStdString() + txt + ".jpg";
    imwrite(path, will_save_frame);

    current_date = current_date_time.toString("yyyyMMdd hh:mm:ss");
    ui_->label_emo->setText(tr(txt.c_str()));
    ui_->label_emo_time->setText(tr(current_date.toLocal8Bit().toStdString().c_str()));
    ui_->statusBar->showMessage(tr("Succeed to save the last frame of frame sequence."));
}

/*
@当表情为消极表情时, 播放音乐; 为积极表情时, 停止播放音乐.
*/
void EmotionAiMainWindow::RelaxMusic(const int emotion_label)
{
    string txt = Lbptop_Expression_Hash_Table[emotion_label];
    if (txt == "anger" || txt == "contempt" || txt == "disgust" || txt == "fear" || txt == "sadness")
    {
        //down_emotion++;
        ui_->label_smile->setPixmap(QPixmap::fromImage(QImage(":/EmotionAI/Resources/boy1.png").
            scaled(51, 51, Qt::KeepAspectRatio)));
        mu.Play();
    }
    else if (txt == "happy" || txt == "surprise")
    {
        ui_->label_smile->setPixmap(QPixmap::fromImage(QImage(":/EmotionAI/Resources/boy2.png").
            scaled(51, 51, Qt::KeepAspectRatio)));
        mu.Stop();
    }
    else
    {
    }
}