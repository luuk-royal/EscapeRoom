#pragma once
#ifndef MAZE_GAME_H
#define MAZE_GAME_H

#include "EscapeMap.h"


// Navigate through a maze to figure out the exit, different from the overworld map but uses the same naming
class MazeGame
{
public:
    MazeGame(LiquidCrystal_I2C &lcd, ButtonWrapper &buttons, EscapeRoomStatus &status, GamesDone &gamesDone);
    void run();
    void updateMap();
private:
    void displayUpdate(byte left[], byte right[]);
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    PlayerLocation playerLocation;
    LiquidCrystal_I2C &lcd;
    ButtonWrapper &buttons;
    EscapeRoomStatus &status;
    GamesDone &gamesDone;
    // switch to decide which rooms players see
    bool lookingUp;

    int mazeMap[8][10] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 2, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 1, 0, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    byte leftSide[8] = {
        0B11111, 
        0B10000,
        0B11101,
        0B10100,
        0B10101,
        0B10000,
        0B10110,
        0B11111
    };

    byte rightSide[8] = {
        0B11111, 
        0B01001, 
        0B11011, 
        0B00001, 
        0B10111, 
        0B01011, 
        0B10001, 
        0B11111
    };
};

#endif // MAZE_GAME_H