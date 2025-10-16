#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "..\..\inc\fmod.h"

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void init();
    void playMenuSound();
    void playEatSound();
    void playHitSound();
    void playWinSound();
    void playLoseSound();
    void stopAllSounds();
    void setMasterVolume(int volume);
    int getMasterVolume();

private:
    FSOUND_STREAM* somMenu;
    FSOUND_SAMPLE* somComeu;
    FSOUND_SAMPLE* somChoque;
    FSOUND_SAMPLE* somParabens;
    FSOUND_SAMPLE* somPerdeu;
};

#endif // SOUNDMANAGER_H
