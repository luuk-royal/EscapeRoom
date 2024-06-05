#include "BalanceGame.h"

BalanceGame::BalanceGame(
    int potmeter, 
    int Led_RRR, 
    int Led_RR, 
    int Led_GR, 
    int Led_YC, 
    int Led_GL, 
    int Led_RL, 
    int Led_RLL
) : POTMETER(potmeter), 
    LED_RRR(Led_RRR), 
    LED_RR(Led_RR), 
    LED_GR(Led_GR), 
    LED_YC(Led_YC), 
    LED_GL(Led_GL), 
    LED_RL(Led_RL), 
    LED_RLL(Led_RLL),
    currentLed(0),  // Start de game met de middelste LED aan
    lastMoveTime(0) 
{}

void BalanceGame::SetUp()
{
    pinMode(POTMETER, INPUT);
    pinMode(LED_RRR, OUTPUT);
    pinMode(LED_RR, OUTPUT);
    pinMode(LED_GR, OUTPUT);
    pinMode(LED_YC, OUTPUT);
    pinMode(LED_GL, OUTPUT);
    pinMode(LED_RL, OUTPUT);
    pinMode(LED_RLL, OUTPUT);
}

void BalanceGame::GameLoop()
{
    int sensorValue = analogRead(POTMETER);
    int mappedValue = map(sensorValue, 0, 1023, -1, 1);

    unsigned long currentTime = millis();
    if (currentTime - lastMoveTime > 500) {  // Beweeg de LED om de 500 milliseconden
        currentLed += mappedValue;
        if (currentLed > 3) currentLed = 3;
        if (currentLed < -3) currentLed = -3;
        lastMoveTime = currentTime;
    }

    digitalWrite(LED_RRR, LOW);
    digitalWrite(LED_RR, LOW);
    digitalWrite(LED_GR, LOW);
    digitalWrite(LED_YC, LOW);
    digitalWrite(LED_GL, LOW);
    digitalWrite(LED_RL, LOW);
    digitalWrite(LED_RLL, LOW);

    switch (currentLed) {
        case -3:
            digitalWrite(LED_RLL, HIGH);
            break;
        case -2:
            digitalWrite(LED_RL, HIGH);
            break;
        case -1:
            digitalWrite(LED_GL, HIGH);
            break;
        case 0:
            digitalWrite(LED_YC, HIGH);
            break;
        case 1:
            digitalWrite(LED_GR, HIGH);
            break;
        case 2:
            digitalWrite(LED_RR, HIGH);
            break;
        case 3:
            digitalWrite(LED_RRR, HIGH);
            break;
    }
}
