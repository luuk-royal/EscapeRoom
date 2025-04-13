#pragma once
#ifndef WHACKAMOLE_H
#define WHACKAMOLE_H

#include <Arduino.h>
#include <TM1638plus.h>
#include "EscapeRoomStates.h"
#include "ButtonWrapper.h"
#include "IGame.h"

struct Mole
{
public:
    unsigned long moleTimer;
    unsigned long moleLastActionTime;
    unsigned long moleCooldownTime;
    MoleStatus state;
};


class WhackAMole: public IGame
{
public:
    WhackAMole(
        TM1638plus &tm,
        ButtonWrapper &buttons,
        EscapeRoomStatus &globalStatus,
        GamesDone &gamesDone,
        LiquidCrystal_I2C &lcd
    );
    void run();
private:
    DifficultyLevel difficultyLevel;
    GameState currentState;
    int points;
    TM1638plus &tm;
    ButtonWrapper &buttons;
    EscapeRoomStatus &globalStatus;
    GamesDone &gamesDone;
    LiquidCrystal_I2C &lcd;

    bool setUpTextDone;

    void setActiveTime(Mole& mole, int ledNumber);
    void endMole(Mole& mole, int ledNumber);
    void resetCooldown(Mole& mole);
    int randomCooldown();

    void setup();
    void running();
    void gameDone();
};

#endif // WHACKAMOLE_H