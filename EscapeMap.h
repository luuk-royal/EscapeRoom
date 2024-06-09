#pragma once
#ifndef ESCAPE_MAP_WRAPPER_H
#define ESCAPE_MAP_WRAPPER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "EscapeRoomStatus.h"
#include "ButtonWrapper.h"

// https://forum.arduino.cc/t/explaination-of-struct/676032/9
// Store the players location in a struct for easy acces, comparisons and updates
struct PlayerLocation {
  public:
    int x;
    int y;
};

class EscapeMap 
{
public:
    EscapeMap(LiquidCrystal_I2C &lcd, ButtonWrapper &buttons, EscapeRoomStatus &status, GamesDone &gamesDone);
    void setup();
    void run();
    void updateMap();
private:
    void displayUpdate(byte left[], byte right[], int playerMapLocation);
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    PlayerLocation playerLocation;
    LiquidCrystal_I2C &lcd;
    ButtonWrapper &buttons;
    EscapeRoomStatus &status;
    GamesDone &gamesDone;
    int lastPlayerMovement;

    int eventMap[8][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 4, 0, 1},
        {1, 0, 3, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 5, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 6, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    byte leftMiniMap[8] = {
        0B11111,
        0B10000,
        0B10000,
        0B10000,
        0B10000,
        0B10000,
        0B10000,
        0B11111
    };

    byte rightMiniMap[8] = {
        0B11111,
        0B00001,
        0B00001,
        0B00001,
        0B00001,
        0B00001,
        0B00001,
        0B11111
    };

    byte topLeftMap[8] = {
        0B00001,
        0B00001,
        0B00001,
        0B00001,
        0B00001,
        0B00001,
        0B00001,
        0B11111
    };

    byte topMidMap[8] = {
        0B10001,
        0B10001,
        0B10001,
        0B10001,
        0B10001,
        0B10001,
        0B10001,
        0B11111
    };
    
    byte topRightMap[8] = {
        0B10000,
        0B10000,
        0B10000,
        0B10000,
        0B10000,
        0B10000,
        0B10000,
        0B11111
    };
    

    byte bottomLeftMap[8] = {
        0B11111,
        0B00001,
        0B00001,
        0B00001,
        0B00001,
        0B00001,
        0B00001,
        0B00001
    };

    byte bottomMidMap[8] = {
        0B11111,
        0B10001,
        0B10001,
        0B10001,
        0B10001,
        0B10001,
        0B10001,
        0B10001,
    };
    
    byte bottomRightMap[8] = {
        0B11111,
        0B10000,
        0B10000,
        0B10000,
        0B10000,
        0B10000,
        0B10000,
        0B10000,
    };

    byte characterMapPart[8] = {
        0B01110,
        0B01110,
        0B00100,
        0B11111,
        0B00100,
        0B00100,
        0B01010,
        0B10001
    };
};

#endif // ESCAPE_MAP_WRAPPER_H