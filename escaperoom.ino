#include <TM1638plus.h>
#include <LiquidCrystal_I2C.h>
#include "ButtonWrapper.h"
#include "Music.h"

#define STB 13
#define CLK 12
#define DIO 11

#define POTMETER A1
#define LED_RRR 2
#define LED_RR 3
#define LED_GR 4
#define LED_YC 6
#define LED_GL 7
#define LED_RL 8
#define LED_RLL 9

#define BUZZER 10
#define FREQUENCY 440

LiquidCrystal_I2C lcd(0x27, 16, 2);
TM1638plus tm(STB, CLK, DIO);
ButtonWrapper buttons(tm);

int lastMilis = 0;

enum escapeRoomStatus {
  inMap = 0,
  inGame1 = 1,
  inGame2 = 2,
  inGame3 = 3,
  inGame4 = 4,
  startingScreen = 5,
  endingScreen = 6
};

byte status = 5;
bool started = false;

// https://forum.arduino.cc/t/explaination-of-struct/676032/9
// Store the players location in a struct for easy acces, comparisons and updates
struct PlayerLocation {
  public:
    int x;
    int y;
};

void startEscapeRoom() {
  Serial.println("In startEscapeRoom!");
  status = 0;
  // mapSetup();
}

// The idea for making the functions the buttons call modular came from here:
// https://codereview.stackexchange.com/questions/210356/improvement-for-stdmap-int-run-some-function-double-dispatch
// https://stackoverflow.com/questions/52934718/c-calling-functions-from-an-array-of-function-pointers
std::function<void()> buttonFunctions[] = {
  startEscapeRoom
};


void setup(void)
{
  Serial.begin(9600);

  tm.displayBegin();
    
  lcd.init();
  delay(500);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("4-bytes-saga");
  lcd.setCursor(5, 1);
  lcd.print(">Start");
}

void loop() {
  // readTMButtons (enabledButtons, buttonFunctions);
  // put your main code here, to run repeatedly:

  // music();
  buttons.updateButtons();

  // Serial.println((int) tm.readButtons());
  // delay(500);

  // tm.setLED(0, 1);

  switch (status) {
    case startingScreen:
      // wait for the first button to be pressed
    break;
    case inMap:
      // updateMap();
    break;
  }

  // tm.reset();
  // for (int i = 0; i < 8; i++) {
  //   tm.setLED(i, 1);
  //   delay(200);
  // }

  // for (int j = 7; j >= 0; j--) {
  //   tm.setLED(j, 0);
  //   delay(200);
  // }

  
}

byte leftMap[] = {
  0B11111,
  0B10000,
  0B10000,
  0B10000,
  0B10000,
  0B10000,
  0B10000,
  0B11111
};

byte rightMap[] = {
  0B11111,
  0B00001,
  0B00001,
  0B00001,
  0B00001,
  0B00001,
  0B00001,
  0B11111
};

// Change player map to X,Y coordinate. then check X,Y coordinate against minigames on moving, else draw character
// Put player in event map, kopie map halves into arrays and draw player

// Map with the events
int eventMap[][10] = {
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,4,0,1},
  {1,0,3,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,5,0,0,0,0,0,1},
  {1,0,0,0,0,6,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1}
};

// Map with the events
int playerMap[][10] = {
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,0,2,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,4,0,1},
  {1,0,3,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,5,0,0,0,0,0,1},
  {1,0,0,0,0,6,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1}
};

void updateMap() {
  // I REALLY wish there was an async version of this but no, so i just have to hope this is fast enough

  // First draft, create the two map parts and then only update the player location
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

  // Run through the left side of the map to check for player position and for special rooms
  for(int i=0; i < 8; i++) {
    for(int j = 0; j < 5; j++) {
      // Once the player has been found exit out of this function
      // i is the Y row of the map, j is the X row of the map
      if (playerMap[i][j] == 2) {
        int temp = 1;
        temp <<= j;
        temp += 1;
        if (4-j > 0) {
          temp << (4-j);
        }

        tempLeftMap[i] = temp;

        // reassing the map
        displayUpdate(tempLeftMap, tempRightMap);

        return;
      }
    } 
  }

  // Run through the right side of the map to check for player position and for special rooms
  for(int i=0; i < 8; i++) {
    for(int j = 5; j < 9; j++) {
      // Once the player has been found exit out of this function
      if (playerMap[i][j] == 2) {
        int temp = 1;
        temp <<= (9 - j);
        temp += 1;

        tempRightMap[i] = temp;

        // reassing the map
        displayUpdate(tempLeftMap, tempRightMap);

        return;
      }
    } 
  }
}

void mapSetup() {
  // TODO: write other 6 blocks here:
  lcd.clear();
  lcd.createChar(0, leftMap);
  lcd.createChar(1, rightMap);
  lcd.setCursor(5, 1);
  lcd.write(0);
  lcd.write(1);
}

// replace the characters in the memory and switch back to the proper memory
void displayUpdate(byte left[], byte right[]) {
  lcd.createChar(0, left);
  lcd.createChar(1, right);
  lcd.setCursor(0, 0);
}

// https://forum.arduino.cc/t/return-struct-from-a-function/277071
// struct PlayerLocation findPlayer() {
//   PlayerLocation returnLocation;

//   // The first and last row (0 and 7) will always be one and the player CAN NEVER be there. So we do not check for them there.
//   for(int i=1; i < 7; i++) {
//     // The first and last horizontal place are walls and can not contain the player
//     for(int j = 1; j < 9; j++) {
//       // Once the player has been found exit out of this function
//       if (playerMap[i][j] == 2) {
//         // i is the Y row of the map, j is the X row of the map
//         returnLocation.y = i;
//         returnLocation.x = j;

//         return returnLocation;
//       }
//     }
//   }

//   // It should never reach this
//   return returnLocation;
// }

void moveLeft() {

}

void moveRight() {

}

void moveUp() {

}

void moveDown() {

}

// Stupid af bitmath
#define firstButton 1
#define secondButton 2
#define thirdButton 4
#define fourthButton 8
#define fifthButton 16
#define sixthButton 32
#define seventhButton 64
#define eigthButton 128

// TODO:
// Wrapper class in CPP around buttons and depending on what i want to do with the buttons react.
// Event based -> debounce and fire event based on pressed button
// State enter game 1 -> wrapper class checks and debounces button -> save button state

// ButtonWrapper class
// UpdateButtons -> reads, debounces and updates internal state
// GetButtonsState -> returns struct of pressed buttons
// GetButtonState(buttonId) -> return one specific button

// Bind enabled buttons and their respective functions to the buttons
void readTMButtons(bool enabled[], std::function<void()> functions[]) {
  int buttonData = tm.readButtons(); // Lees de gecombineerde waarde van alle ingedrukte knoppen

  // Check every button serperately and call the related function
  if (buttonData & firstButton && enabled[0]) {
    // https://cplusplus.com/forum/beginner/4639/#:~:text=Yeah%2C%20you%20totally%20can.
    functions[0]();
  }
  if (buttonData & secondButton && enabled[1]) {
    functions[1]();
  }
  if (buttonData & thirdButton && enabled[2]) {
    functions[2]();
  }
  if (buttonData & fourthButton && enabled[3]) {
    functions[3]();
  }
  if (buttonData & fifthButton && enabled[4]) {
    functions[4]();
  }
  if (buttonData & sixthButton && enabled[5]) {
    functions[5]();
  }
  if (buttonData & seventhButton && enabled[6]) {
    functions[6]();
  }
  if (buttonData & eigthButton && enabled[7]) {
    functions[7]();
  }

}

