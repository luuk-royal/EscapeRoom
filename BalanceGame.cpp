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
    LiquidCrystal_I2C &lcd,
    ButtonWrapper &buttons
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
    buttons(buttons)
{
    // Set the initial state
    currentState = initializing;
}

void BalanceGame::setup()
{
    pinMode(POTMETER, INPUT);
    pinMode(LED_RRR, OUTPUT);
    pinMode(LED_RR, OUTPUT);
    pinMode(LED_GR, OUTPUT);
    pinMode(LED_YC, OUTPUT);
    pinMode(LED_GL, OUTPUT);
    pinMode(LED_RL, OUTPUT);
    pinMode(LED_RLL, OUTPUT);

    currentLed = 0; // Start the game with the middle LED on
    lastMoveTime = 0;
    lastTimed = 0;
}

void BalanceGame::run() {
    switch (currentState)
    {
    case initializing:
        // Only run this text once to prevent flikkering on the screen
        if (!setUpTextDone)
        {
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("balance game");
            lcd.setCursor(5, 1);
            lcd.print(">Start");

            setUpTextDone = true;
        }
        

        // Check if the user has started the game
        if (buttons.getButtonState(1))
        {
            setup();
            currentState = inGame;

            startingTime = millis();

            // Tell the ingame state to set up the text and runtime related variables on frame 0 of the game
            setUpTextDone = false;
        }

        break;
    case inGame:
        running();
        break;
    case done:
        gameDone();
        break;
    case failed:
        gameFailed();
        break;
    }
}

void BalanceGame::running()
{
    // Get reusable value instead of constantly calling millis() - startingTime
    int timePlayed = millis() - startingTime;

    // Only update the timer every second or when the timer has been reset because the minigame was failed
    if ((millis() - lastTimed) > 1000 || !setUpTextDone)
    {
        // After the first loop everything is set up properly timing wise so ignore setUpTextDone unless the game resets
        if (!setUpTextDone) setUpTextDone = true;
        
        lastTimed = millis();
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Started");
        lcd.setCursor(5,1);
        lcd.print(timePlayed / 1000);
        // todo: check if this works as intended
        lcd.setCursor(7,1);
        lcd.print("/20");
    }
    

    // Game lasts 20 seconds
    if (timePlayed > 20000)
    {
        setUpTextDone = false;
        currentState = done;
        // Cancel out of the loop
        return;
    }

    int sensorValue = analogRead(POTMETER);
    int mappedValue = map(sensorValue, 0, 1023, -1, 1);

    unsigned long currentTime = millis();
    // difficulty does not go under 0.5 seconds since it is already extremely easy to fail this minigame with the LED positions changing every 0.5 seconds
    unsigned long moveInterval = (timePlayed <= 5000) ? 1000 : 500;

    if ((currentTime - lastMoveTime) > moveInterval) {  // Move the LED every 500 milliseconds
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
            currentState = failed;
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
            currentState = failed;
            break;
    }
}

void BalanceGame::gameFailed()
{
    // Set all pins to low
    digitalWrite(LED_RRR, LOW);
    digitalWrite(LED_RR, LOW);
    digitalWrite(LED_GR, LOW);
    digitalWrite(LED_YC, LOW);
    digitalWrite(LED_GL, LOW);
    digitalWrite(LED_RL, LOW);
    digitalWrite(LED_RLL, LOW);

    // Reset game values and set the state back to the starting screen of the game.
    currentState = initializing;
    currentLed = 0;
    setUpTextDone = false;
}

void BalanceGame::gameDone() {
    if (!setUpTextDone)
    {
        setUpTextDone = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You win! You got");

        lcd.setCursor(0, 1);
        lcd.print("The Compas");
    }
    

    // Check if the user has decided to go back to the map
    if (buttons.getButtonState(1))
    {
        // Tell the main loop to start running the map code again
        status = inMap;

        // Set this game to done
        gamesDone.gameOneDone = true;

        // Set all pins to low
        digitalWrite(LED_RRR, LOW);
        digitalWrite(LED_RR, LOW);
        digitalWrite(LED_GR, LOW);
        digitalWrite(LED_YC, LOW);
        digitalWrite(LED_GL, LOW);
        digitalWrite(LED_RL, LOW);
        digitalWrite(LED_RLL, LOW);
    }
}