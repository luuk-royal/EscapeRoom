#pragma once
#ifndef WHACKAMOLE_H
#define WHACKAMOLE_H

#include <Arduino.h>
#include <TM1638plus.h>

class WhackAMole
{
public:
    WhackAMole(TM1638plus &tm);
    void SetUp();
    void Run();
private:
    TM1638plus &tm; // Reference to the TM1638plus object
};

#endif // WHACKAMOLE_H