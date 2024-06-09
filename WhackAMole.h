#pragma once
#ifndef WHACKAMOLE_H
#define WHACKAMOLE_H

#include <Arduino.h>
#include <TM1638plus.h>
#include "EscapeRoomStatus.h"
#include "ButtonWrapper.h"

class WhackAMole
{
public:
    WhackAMole(TM1638plus &tm, ButtonWrapper &buttons, EscapeRoomStatus &status, GamesDone &gamesDone);
    void run();
private:
    int difficultyLevel;
    int points;

    TM1638plus &tm;
    ButtonWrapper &buttons;
    EscapeRoomStatus &status;
    GamesDone &gamesDone;

    void gameDone();
    int randomCooldown();
};

#endif // WHACKAMOLE_H