#include "BalanceGame.h"

BalanceGame::BalanceGame(
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
) : POTMETER(potmeter), 
    LED_RRR(Led_RRR), 
    LED_RR(Led_RR), 
    LED_GR(Led_GR), 
    LED_YC(Led_YC), 
    LED_GL(Led_GL), 
    LED_RL(Led_RL), 
    LED_RLL(Led_RLL),
    status(status),
    gamesDone(gamesDone),
    lcd(lcd),
    currentLed(0),  // Start the game with the middel LED on
    lastMoveTime(0),
    lastTimed(0)
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

void BalanceGame::Run()
{
    if (startingTime == -1)
    {
        startingTime = millis();
        lastTimed = millis();
    }

    // Get reusable value instead of constantly calling millis() - startingTime
    int timePlayed = millis() - startingTime;

    // Only update the timer every second or when the timer has been reset because the minigame was failed
    if ((millis() - lastTimed) > 1000)
    {
        lastTimed = millis();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(timePlayed / 1000);
    }
    

    // Game lasts 20 seconds
    if (timePlayed > 20000)
    {
        GameDone();
        // Cancel about the loop
        return;
    }
    

    int sensorValue = analogRead(POTMETER);
    // The maximum sensor value was detected to be 140 so clamp the values to 140
    int mappedValue = map(sensorValue, 0, 1023, -1, 1);

    unsigned long currentTime = millis();
    if (currentTime - lastMoveTime > 500) {  // Move the LED every 500 milliseconds
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
            GameFailed();
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
            GameFailed();
            break;
    }
}

void BalanceGame::GameFailed()
{
    // Reset game values
    startingTime = -1;
    currentLed = 0;
    lastTimed = millis();

    // Print the timer being at 0 again.
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(0);
}

void BalanceGame::GameDone() {
    // Set all pins to low
    digitalWrite(LED_RRR, LOW);
    digitalWrite(LED_RR, LOW);
    digitalWrite(LED_GR, LOW);
    digitalWrite(LED_YC, LOW);
    digitalWrite(LED_GL, LOW);
    digitalWrite(LED_RL, LOW);
    digitalWrite(LED_RLL, LOW);

    // Set this game to done
    gamesDone.gameOneDone = true;

    // Tell the main loop to start running the map code again
    status = inMap;
}