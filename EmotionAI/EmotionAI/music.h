/*
@≤•∑≈“Ù¿÷¿‡
*/
#ifndef MUSIC_H
#define MUSIC_H
#include <QtMultimedia/QSoundEffect>

class Music
{
public:
    Music();
    virtual ~Music();

    void Init();
    void Play();
    void Stop();

private:
    QSoundEffect *qt_audio_player_;
};
#endif /*MUSIC_H*/