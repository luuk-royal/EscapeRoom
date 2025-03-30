#include "MazeGame.h"

bool setupDone = false;
int currentTime = -1; 
int lastPlayerMovement;

MazeGame::MazeGame(
    LiquidCrystal_I2C &lcd, 
    ButtonWrapper &buttons, 
    EscapeRoomStatus &status, 
    GamesDone &gamesDone
    ) : lcd(lcd),
    buttons(buttons),
    status(status),
    gamesDone(gamesDone),
    playerLocation({1, 1})
{
}

void MazeGame::run()
{
    if (currentTime = -1)
    {
        currentTime = millis();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please finish");
        lcd.setCursor(0, 1);
        lcd.print("Game 1 and 2");
    }
    
    if (gamesDone.gameOneDone && gamesDone.gameTwoDone)
    {
        if (setupDone)
        {
            lcd.createChar(0, leftSide);
            lcd.createChar(1, rightSide);
            lcd.setCursor(0, 0);
            lcd.write(0);
            lcd.write(1);
        }
        
        if ((millis() - lastPlayerMovement) < 500) {
        } else {
            lastPlayerMovement = millis();
            ButtonState buttonState = buttons.getButtonsState();
            // Only do something if a button is being pressed, ignore all other inputs outside of the first one found
            if (buttonState.buttonOnePressed)
            {
            moveLeft();
            } else if (buttonState.buttonTwoPressed)
            {
            moveRight();
            } else if (buttonState.buttonThreePressed)
            {
            moveUp();
            } else if (buttonState.buttonFourPressed)
            {
            moveDown();
            }
        }
    } else {
        // display message for 5 seconds then go back to map
        if ((millis() - currentTime) > 5000)
        {
            status = inMap;
        }
    } 
}

void MazeGame::updateMap() 
{
    byte tempLeft[8] = {
        0B11111, 
        0B10000,
        0B11101,
        0B10100,
        0B10101,
        0B10000,
        0B10110,
        0B11111
    };

    byte tempRight[8] = {
        0B11111, 
        0B01001, 
        0B11011, 
        0B00001, 
        0B10111, 
        0B01011, 
        0B10001, 
        0B11111
    };

    int x = playerLocation.x;
    int y = playerLocation.y;

    if (x < 5) {
        tempLeft[y] |= (1 << (4 - x));
    } else {
        int invertedX = 9 - x;
        tempRight[y] |= (1 << invertedX);
    }

    displayUpdate(tempLeft, tempRight);
}

void MazeGame::displayUpdate(byte left[], byte right[])
{
    lcd.createChar(0, left);
    lcd.createChar(1, right);
    // Go out of volotile memory
    lcd.setCursor(0,1);
}

void MazeGame::moveLeft()
{
    int x = playerLocation.x;

    if (mazeMap[playerLocation.y][x - 1] != 1) {
        playerLocation.x = x - 1;
        if (mazeMap[playerLocation.y][x - 1] == 2)
        {
            status = endingScreen;
        }
    }
}

void MazeGame::moveRight() {
    int x = playerLocation.x;

    if (mazeMap[playerLocation.y][x + 1] != 1) {
        playerLocation.x = x + 1;
        if (mazeMap[playerLocation.y][x + 1] == 2)
        {
            status = endingScreen;
        }
    }
}

void MazeGame::moveUp() {
    int y = playerLocation.y;

    if (mazeMap[y - 1][playerLocation.x] != 1) {
        playerLocation.y = y - 1;
        if (mazeMap[y - 1][playerLocation.x])
        {
            status = endingScreen;
        }
    }
}

void MazeGame::moveDown() {
    int y = playerLocation.y;

    if (mazeMap[y + 1][playerLocation.x] != 1) {
        playerLocation.y = y + 1;
        if (mazeMap[y + 1][playerLocation.x])
        {
            status = endingScreen;
        }
    }
}