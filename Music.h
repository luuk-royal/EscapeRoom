#pragma once
#ifndef MUSIC_WRAPPER_H
#define MUSIC_WRAPPER_H

#include <Arduino.h>

class Music
{
public:
    Music(int port);
    void PlayMusic();
    void SwitchTrack(int trackId);
    void ResetMusic();
private:
    int currentTrack;
    int buzzer;
};

#endif // MUSIC_WRAPPER_H