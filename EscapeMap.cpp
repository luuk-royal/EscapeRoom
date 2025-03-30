#include "EscapeMap.h"

EscapeMap::EscapeMap(LiquidCrystal_I2C &lcd, 
ButtonWrapper &buttons, 
EscapeRoomStatus &status, 
GamesDone &gamesDone
) : lcd(lcd), 
buttons(buttons), 
status(status), 
gamesDone(gamesDone), 
playerLocation({4, 1}) 
{
}

void EscapeMap::run()
{
  // Debug stub for player movement:
  // Serial.print("x: ");
  // Serial.print(playerLocation.x);
  // Serial.print(", y: ");
  // Serial.println(playerLocation.y);

  // Move once per second, start on cooldown to assure no input from the menu or games pollutes the map movement
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
    updateMap();
  }
}

void EscapeMap::updateMap() {
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
  
  if (x < 5) {
      tempLeftMap[y] |= (1 << (4 - x));
  } else {
      // Invert the x coordinate for the right map
      int invertedX = 9 - x;  // Invert x, so 6 becomes 3, 7 becomes 2, etc.
      tempRightMap[y] |= (1 << invertedX);
  }

  int playerMapLocation;

  if (x < 4) {
    playerMapLocation = (y < 5) ? 2 : 5;
  } else if (x == 4 || x == 5) {
      playerMapLocation = (y < 4) ? 3 : 6;
  } else {
      playerMapLocation = (y < 4) ? 4 : 7;
  }

  // reassign the map
  displayUpdate(tempLeftMap, tempRightMap, playerMapLocation);
}

// put back the characters in the memory, switch back to the proper memory and rewrite the map
void EscapeMap::displayUpdate(byte mapLeft[], byte mapRight[], int playerMapLocation) {
  // The minimap was made first so it retains the 0 and 1 character positions respectively
  lcd.createChar(0, mapLeft);
  lcd.createChar(1, mapRight);
  lcd.createChar(2, topLeftMap);
  lcd.createChar(3, topMidMap);
  lcd.createChar(4, topRightMap);
  lcd.createChar(5, bottomLeftMap);
  lcd.createChar(6, bottomMidMap);
  lcd.createChar(7, bottomRightMap);

  //  overwrite the map segment with the player segment
  lcd.createChar(playerMapLocation, characterMapPart);
  
  // Clear lcd of minigames/menus
  lcd.clear();

  // write the map
  lcd.setCursor(0, 0);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  
  lcd.setCursor(0, 1);
  lcd.write(5);
  lcd.write(6);
  lcd.write(7);

  // Display amount of games finished
  int gamesFinished = gamesDone.gameOneDone + gamesDone.gameTwoDone + gamesDone.gameThreeDone + gamesDone.gameFourDone;
  lcd.setCursor(4, 0);
  lcd.print(gamesFinished);
  lcd.print('/');
  lcd.print(2); // Probably should not say 3 as the third minigame is the exit

  lcd.setCursor(5, 1);
  lcd.write(0);
  lcd.write(1);
}

void EscapeMap::moveLeft() {
    int x = playerLocation.x;

    if (eventMap[playerLocation.y][x - 1] != 1) {
        playerLocation.x = x - 1;
        switch (eventMap[playerLocation.y][x - 1]) {
            // Only go into a game when it is not done
            case 3: if (!gamesDone.gameOneDone) status = inGame1; break;
            case 4: if (!gamesDone.gameTwoDone) status = inGame2; break;
            case 5: if (!gamesDone.gameThreeDone) status = inGame3; break;
            // case 6: if (!gamesDone.gameFourDone) status = inGame4; break;
            default: status = inMap; break;
        }
    }
}

void EscapeMap::moveRight() {
    int x = playerLocation.x;

    if (eventMap[playerLocation.y][x + 1] != 1) {
        playerLocation.x = x + 1;
        switch (eventMap[playerLocation.y][x + 1]) {
            // Only go into a game when it is not done
            case 3: if (!gamesDone.gameOneDone) status = inGame1; break;
            case 4: if (!gamesDone.gameTwoDone) status = inGame2; break;
            case 5: if (!gamesDone.gameThreeDone) status = inGame3; break;
            // case 6: if (!gamesDone.gameFourDone) status = inGame4; break;
            default: status = inMap; break;
        }
    }
}

void EscapeMap::moveUp() {
    int y = playerLocation.y;

    if (eventMap[y - 1][playerLocation.x] != 1) {
        playerLocation.y = y - 1;
        switch (eventMap[y - 1][playerLocation.x]) {
            // Only go into a game when it is not done
            case 3: if (!gamesDone.gameOneDone) status = inGame1; break;
            case 4: if (!gamesDone.gameTwoDone) status = inGame2; break;
            case 5: if (!gamesDone.gameThreeDone) status = inGame3; break;
            // case 6: if (!gamesDone.gameFourDone) status = inGame4; break;
            default: status = inMap; break;
        }
    }
}

void EscapeMap::moveDown() {
    int y = playerLocation.y;

    if (eventMap[y + 1][playerLocation.x] != 1) {
        playerLocation.y = y + 1;
        switch (eventMap[y + 1][playerLocation.x]) {
            // Only go into a game when it is not done
            case 3: if (!gamesDone.gameOneDone) status = inGame1; break;
            case 4: if (!gamesDone.gameTwoDone) status = inGame2; break;
            case 5: if (!gamesDone.gameThreeDone) status = inGame3; break;
            // case 6: if (!gamesDone.gameFourDone) status = inGame4; break;
            default: status = inMap; break;
        }
    }
}