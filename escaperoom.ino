#include <TM1638plus.h>
#include <LiquidCrystal_I2C.h>
#include "EscapeRoomStatus.h"
#include "ButtonWrapper.h"
#include "Music.h"
#include "EscapeMap.h"

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
Music music(BUZZER);
EscapeMap map(lcd, buttons, status);

int lastMilis = 0;

byte status = 5;
bool started = false;

void startEscapeRoom() {
  Serial.println("In startEscapeRoom!");
  status = 0;
  map.Setup();
}

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
  music.PlayMusic();

  // Serial.println((int) tm.readButtons());
  // delay(500);

  // tm.setLED(0, 1);
  ButtonState buttonState = buttons.getButtonsState();

  switch (status) {
    case startingScreen:
      if (buttons.getButtonState(1))
      {
        startEscapeRoom();
      }
      

    break;
    case inMap:
      map.Run();
      
      break;
    case inGame1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("In game one");
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
