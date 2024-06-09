#pragma once
#ifndef BALANCE_GAME_H
#define BALANCE_GAME_H

#include <Arduino.h>
#include "EscapeRoomStatus.h"
#include <LiquidCrystal_I2C.h>

class BalanceGame
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
        LiquidCrystal_I2C &lcd
    );
    void SetUp();
    void Run();
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
    LiquidCrystal_I2C &lcd;

    void GameFailed();
    void GameDone();
};

#endif // BALANCE_GAME_H