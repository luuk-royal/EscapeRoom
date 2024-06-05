#include "EscapeMap.h"

EscapeMap::Map(LiquidCrystal_I2C &lcd, ButtonWrapper &buttons, EscapeRoomStatus &status) 
    : lcd(lcd), buttons(buttons), status(status), playerLocation({5, 2}) {}

void EscapeMap::Setup()
{
    // TODO: write other 6 blocks here:
    lcd.clear();
    lcd.createChar(0, leftMap);
    lcd.createChar(1, rightMap);
    lcd.setCursor(5, 1);
    lcd.write(0);
    lcd.write(1);

    // The players starting location is ALWAYS (5,2)
    playerLocation.x = 5;
    playerLocation.y = 2;
}

void EscapeMap::Run()
{
    // Only do something if a button is being pressed, ignore all other inputs outside of the first one found
        if (buttonState.buttonOnePressed)
        {
        moveLeft();
        } else if (buttonState.buttonTwoPressed)
        {
        moveUp();
        } else if (buttonState.buttonThreePressed)
        {
        moveDown();
        } else if (buttonState.buttonFourPressed)
        {
        moveRight();
        }
}

void EscapeMap::UpdateMap() {
  // Create the two map parts and add the player to the map half using the player location.
  // Also check if player has entered an event tile, if so give the player the ability to enter the game.
  
  byte tempLeftMap[] = {
    0B11111,
    0B10000,
    0B10000,
    0B10000,
    0B10000,
    0B10000,
    0B10000,
    0B11111
  };

  byte tempRightMap[] = {
    0B11111,
    0B00001,
    0B00001,
    0B00001,
    0B00001,
    0B00001,
    0B00001,
    0B11111
  };

  // Byte shifting
  // https://www.geeksforgeeks.org/left-shift-right-shift-operators-c-cpp/

  int x = playerLocation.x;
  int y = playerLocation.y;
  if (x < 6)
  {
    int temp = 1;
    temp <<= x;
    temp += 1;
    if (4-x > 0) {
      temp << (4-x);
    }

    tempLeftMap[y] = temp;

  } else {
    int temp = 1;
    temp <<= (9 - x);
    temp += 1;

    tempRightMap[y] = temp;
  }

  // reassign the map
  DisplayUpdate(tempLeftMap, tempRightMap);
}

// replace the characters in the memory and switch back to the proper memory
void EscapeMap::DisplayUpdate(byte left[], byte right[]) {
  lcd.createChar(0, left);
  lcd.createChar(1, right);
  lcd.setCursor(0, 0);
}

void EscapeMap::MoveLeft() {
    int x = playerLocation.x;

    if (eventMap[playerLocation.y][x - 1] != 1) {
        playerLocation.x = x - 1;
        switch (eventMap[playerLocation.y][x - 1]) {
            case 3: status = inGame1; break;
            case 4: status = inGame2; break;
            case 5: status = inGame3; break;
            case 6: status = inGame4; break;
            default: status = inMap; break;
        }
        UpdateMap();
    }
}

void EscapeMap::MoveRight() {
    int x = playerLocation.x;

    if (eventMap[playerLocation.y][x + 1] != 1) {
        playerLocation.x = x + 1;
        switch (eventMap[playerLocation.y][x + 1]) {
            case 3: status = inGame1; break;
            case 4: status = inGame2; break;
            case 5: status = inGame3; break;
            case 6: status = inGame4; break;
            default: status = inMap; break;
        }
        UpdateMap();
    }
}

void EscapeMap::MoveUp() {
    int y = playerLocation.y;

    if (eventMap[y - 1][playerLocation.x] != 1) {
        playerLocation.y = y - 1;
        switch (eventMap[y - 1][playerLocation.x]) {
            case 3: status = inGame1; break;
            case 4: status = inGame2; break;
            case 5: status = inGame3; break;
            case 6: status = inGame4; break;
            default: status = inMap; break;
        }
        UpdateMap();
    }
}

void EscapeMap::MoveDown() {
    int y = playerLocation.y;

    if (eventMap[y + 1][playerLocation.x] != 1) {
        playerLocation.y = y + 1;
        switch (eventMap[y + 1][playerLocation.x]) {
            case 3: status = inGame1; break;
            case 4: status = inGame2; break;
            case 5: status = inGame3; break;
            case 6: status = inGame4; break;
            default: status = inMap; break;
        }
        UpdateMap();
    }
}