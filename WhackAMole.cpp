#include "WhackAMole.h"

// Define eight moles
Mole moles[8];

WhackAMole::WhackAMole(
    TM1638plus &tm,
    ButtonWrapper &buttons,
    EscapeRoomStatus &status,
    GamesDone &gamesDone,
    LiquidCrystal_I2C &lcd,
    ButtonWrapper &buttons
    ) : tm(tm),
    buttons(buttons),
    status(status),
    gamesDone(gamesDone),
    lcd(lcd),
    buttons(buttons),
    currentState(initializing),
    setUpTextDone(false)
{
}

void WhackAMole::run()
{
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
        
        setup();
        break;
    case inGame:
        running();
        break;
    case done:
        gameDone();
        break;
    default:
        break;
    }

    // Change the difficulty based on points gained
    if (points < 5)
    {
        difficultyLevel = easy;
    } else if (points < 15)
    {
        difficultyLevel = medium;
    } else if (points < 25)
    {
        difficultyLevel = hard;
    } else {
        currentState = done;
        setUpTextDone = false;
    }
}

void WhackAMole::setup() {
    for (int i = 0; i < 8; i++)
    {
        moles[i] = Mole();
        resetCooldown(moles[i]);
    }
    
    difficultyLevel = easy;
    points = 0;
}

void WhackAMole::running() {
    unsigned long currentMillis = millis();
    ButtonState buttonState = buttons.getButtonsState();  // Get the current state of all buttons
    
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
        lcd.print(points);
        // todo: check if this works as intended
        lcd.setCursor(7,1);
        lcd.print("/25");
    }

    // Run game logic for all 8 moles mole
    for (int i = 0; i < 8; i++)
    {
        // Get a reference to the mole
        Mole& mole = moles[i];

        // Check if the mole buttons has been pressed while the mole was not on
        if (buttons.getButtonState(i) && mole.state != started)
        {
            if (points > 0) points--;
            endMole(mole, i);
        }

        switch (mole.state)
        {
            case coolingDown:
                if ((currentMillis - mole.moleLastActionTime) > mole.moleCooldownTime)
                {
                    setActiveTime(mole, i);
                }
                
                break;
            case started:

                if (buttons.getButtonState(i))
                {
                    points++;
                    endMole(mole, i);
                }
                

                if ((currentMillis - mole.moleLastActionTime) > mole.moleTimer)
                {
                    if (points > 0) points--;
                    endMole(mole, i);
                }
                    
            
                break;
            case ended:
            default:
                resetCooldown(moles[i]);
                break;
        }
    }
}

void WhackAMole::gameDone() {
    if (!setUpTextDone)
    {
        // Reset the tm LEDS
        tm.reset();

        setUpTextDone = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You win! You got");

        lcd.setCursor(0, 1);
        lcd.print("The Hammer");
    }

    // Check if the user has decided to go back to the map
    if (buttons.getButtonState(1))
    {
        // Tell the main loop to start running the map code again
        status = inMap;

        // Set this game to done
        gamesDone.gameTwoDone = true;
    }
}

void WhackAMole::setActiveTime(Mole& mole, int i) {
    mole.moleLastActionTime = millis();

    // Set the active time for the mole based on difficulty
    switch (difficultyLevel)
    {
        case hard:
            mole.moleTimer = 250;
            break;
        case medium:
            mole.moleTimer = 500;
            break;
        case easy:
        default: // aka difficulty one
            mole.moleTimer = 1000;
            break;
    }

    mole.state = started;
    tm.setLED(i, 1);    // Turn the light that represents the mole on
}

void WhackAMole::endMole(Mole& mole, int i) {
    mole.state = ended;
    tm.setLED(i, 0);    // Turn the light that represents the mole off
}

void WhackAMole::resetCooldown(Mole& mole) {
    switch (difficultyLevel)
    {
        case hard:
            mole.moleCooldownTime = random(750, 3000);
            break;
        case medium:
            mole.moleCooldownTime = random(500, 2000);
            break;
        // make the default also the first difficulty so a faulty state doesn't break the game
        case easy:
        default: 
            mole.moleCooldownTime = random(1000, 4000);
        break;
    }

    mole.moleLastActionTime = millis();
    mole.state = coolingDown;
}

int WhackAMole::randomCooldown()
{
    switch (difficultyLevel)
    {
        default: // aka difficulty one
            return random(1000, 4000);
        case 2:
            return random(750, 3000);
        case 3:
            return random(500, 2000);
    }
}