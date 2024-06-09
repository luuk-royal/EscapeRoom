#include "WhackAMole.h"

// Define timers and cooldowns for eight moles
int moleTimers[8];
int molesLastActive[8];
int moleCooldowns[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
bool moleStarted[8] = {false, false, false, false, false, false, false, false};

WhackAMole::WhackAMole(
    TM1638plus &tm, 
    ButtonWrapper &buttons, 
    EscapeRoomStatus &status, 
    GamesDone &gamesDone
    ) : tm(tm),
    buttons(buttons),
    status(status),
    gamesDone(gamesDone),
    difficultyLevel(1), 
    points(0)
{
}

void WhackAMole::run()
{
    // Change the difficulty based on points gained
    if (points < 5)
    {
        difficultyLevel = 1;
    } else if (points < 15)
    {
        difficultyLevel = 2;
    } else if (points < 25)
    {
        difficultyLevel = 3;
    } else {
        gameDone();
    }
    
    
    for (int i = 0; i < 8; i++)
    {
        if (moleCooldowns[i] == -1)
        {
            switch (difficultyLevel)
            {
                default: // aka difficulty one
                    moleCooldowns[i] = random(1000, 4000);
                    break;
                case 2:
                    moleCooldowns[i] = random(750, 3000);
                    break;
                case 3:
                    moleCooldowns[i] = random(500, 2000);
                    break;
            }
        }
    }
    
    // Run game logic for each mole
    unsigned long currentMillis = millis();
    ButtonState buttonState = buttons.getButtonsState();  // Get the current state of all buttons

    for (int i = 0; i < 8; i++)
    {
        // Ensure initial delay of at least one second
        if (!moleStarted[i])
        {
            if ((currentMillis - molesLastActive[i]) < 1000)
            {
                tm.setLED(i, 0);  // Ensure the mole LED is off
                continue;
            }
            moleStarted[i] = true;
            molesLastActive[i] = currentMillis;
            moleCooldowns[i] = -1;  // Ready to start the game logic
        }

        // If the mole is currently active
        if ((currentMillis - molesLastActive[i]) < moleTimers[i])
        {
            bool buttonPressed = false;
            switch(i)
            {
                case 0: buttonPressed = buttonState.buttonOnePressed; break;
                case 1: buttonPressed = buttonState.buttonTwoPressed; break;
                case 2: buttonPressed = buttonState.buttonThreePressed; break;
                case 3: buttonPressed = buttonState.buttonFourPressed; break;
                case 4: buttonPressed = buttonState.buttonFivePressed; break;
                case 5: buttonPressed = buttonState.buttonSixPressed; break;
                case 6: buttonPressed = buttonState.buttonSevenPressed; break;
                case 7: buttonPressed = buttonState.buttonEightPressed; break;
            }

            if (buttonPressed)
            {
                points++;
                tm.setLED(i, 0);  // Turn the light that represents the mole off
                moleTimers[i] = 0;     // Reset the mole timer
                molesLastActive[i] = currentMillis;  // Reset the last active time
                moleCooldowns[i] = randomCooldown(); // Set a new cooldown
                tm.displayIntNum(points);
            }
        } 
        // If the mole timer has expired, deactivate the mole and start the cooldown
        else if ((currentMillis - molesLastActive[i]) >= moleTimers[i] && moleTimers[i] > 0)
        {
            tm.setLED(i, 0);  // Turn off the mole LED
            moleTimers[i] = 0;  // Reset the mole timer
            molesLastActive[i] = currentMillis;  // Reset the last active time
            moleCooldowns[i] = randomCooldown(); // Set a new cooldown
        }

        // If the cooldown period has passed, activate a new mole
        if ((currentMillis - molesLastActive[i]) >= moleCooldowns[i] && moleCooldowns[i] != -1)
        {
            molesLastActive[i] = currentMillis;

            // Set the active time for the mole based on difficulty
            switch (difficultyLevel)
            {
                default: // aka difficulty one
                    moleTimers[i] = 1000;
                    break;
                case 2:
                    moleTimers[i] = 500;
                    break;
                case 3:
                    moleTimers[i] = 250;
                    break;
            }
            
            moleCooldowns[i] = -1;  // Reset the cooldown
            tm.setLED(i, 1);    // Turn the light that represents the mole on
        }
    }
}

void WhackAMole::gameDone() {
    // Reset the tm LEDS
    tm.reset();

    // Set this game to done
    gamesDone.gameTwoDone = true;

    // Tell the main loop to start running the map code again
    status = inMap;
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