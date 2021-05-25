#include <string>

#include "music.h"
using namespace std;

Music::Music()
{
    qt_audio_player_ = new QSoundEffect;
}

Music::~Music()
{
    delete qt_audio_player_;
}

void Music::Init()
{
    string str = "music\\joy.wav";
    QString q_file_path = QString::fromStdString(str);
    
    qt_audio_player_->setLoopCount(1);
    qt_audio_player_->setVolume(1.0f);
    qt_audio_player_->setSource(QUrl::fromLocalFile(q_file_path));
}

void Music::Play()
{
   qt_audio_player_->play();
}

void Music::Stop()
{
    qt_audio_player_->stop();
}