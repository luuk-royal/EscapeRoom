#pragma once
#ifndef MUSIC_WRAPPER_H
#define MUSIC_WRAPPER_H

#include <Arduino.h>

class Music
{
public:
    void PlayMusic();
    void SwitchTrack(int trackId);
    void ResetMusic();
private:
    int currentTrack
}

#endif // MUSIC_WRAPPER_H