#pragma once
#ifndef BALANCE_GAME_H
#define BALANCE_GAME_H

#include <Arduino.h>

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
        int Led_RLL
    );
    void SetUp();
    void GameLoop();
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
};

#endif // BALANCE_GAME_H