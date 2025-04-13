#pragma once
#ifndef BALANCE_GAME_H
#define BALANCE_GAME_H

#include <Arduino.h>
#include "EscapeRoomStates.h"
#include <LiquidCrystal_I2C.h>
#include "ButtonWrapper.h"
#include "IGame.h"

class BalanceGame: public IGame
{
public:
    BalanceGame(
        int potmeter,
        int Led_RRR,
        int Led_RR,
        int Led_GR,
        int Led_YC,
        int Led_GL,
        int Led_RL,
        int Led_RLL,
        EscapeRoomStatus &status,
        GamesDone &gamesDone,
        LiquidCrystal_I2C &lcd,
        ButtonWrapper &buttons
    );
    void run();
private:
    int POTMETER;
    int LED_RRR;
    int LED_RR;
    int LED_GR;
    int LED_YC;
    int LED_GL;
    int LED_RL;
    int LED_RLL;
    int currentLed;
    unsigned long lastMoveTime;
    int startingTime = -1;
    int lastTimed;
    EscapeRoomStatus &status;
    GamesDone &gamesDone;
    GameState currentState;
    LiquidCrystal_I2C &lcd;
    ButtonWrapper &buttons;
    int startTimer;

    bool setUpTextDone;

    void setup();
    void running();
    void gameDone();
    void gameFailed();
};

#endif // BALANCE_GAME_H