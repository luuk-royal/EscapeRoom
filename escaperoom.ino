#include <TM1638plus.h>
#include <LiquidCrystal_I2C.h>
#include "EscapeRoomStates.h"
#include "ButtonWrapper.h"
#include "Music.h"
#include "EscapeMap.h"
#include "BalanceGame.h"
#include "WhackAMole.h"
// #include "MazeGame.h"
// #include "ParcourGame.h"

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

int lastMilis = 0;
EscapeRoomStatus status = startingScreen;
// 5 minutes
int totalTime = 300000;
// Only start counting down once the startEscapeRoom function has been gone through
int startingMillis = 0;
bool hasEnded = false;
int timeLeft;

GamesDone gamesDone;

LiquidCrystal_I2C lcd(0x27, 16, 2);
TM1638plus tm(STB, CLK, DIO);
ButtonWrapper buttons(tm);
Music music(BUZZER);
EscapeMap escapeMap(lcd, buttons, status, gamesDone);
BalanceGame gameOne(POTMETER, LED_RRR, LED_RR, LED_GR, LED_YC, LED_GL, LED_RL, LED_RLL, status, gamesDone, lcd, buttons);
WhackAMole gameTwo(tm, buttons, status, gamesDone, lcd);
// ParcourGame gameThree(lcd, buttons, status, gamesDone);
// MazeGame gameFour(lcd, buttons, status, gamesDone);

void startEscapeRoom() {
  // Serial.println("In startEscapeRoom!");
  status = inMap;
  escapeMap.setup();
  
  startingMillis = millis();
}

void setup(void)
{
  Serial.begin(9600);

  tm.displayBegin();
    
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("4-bytes-saga");
  lcd.setCursor(5, 1);
  lcd.print(">Start");
}

void loop() {
  buttons.updateButtons();
  music.PlayMusic();


  ButtonState buttonState = buttons.getButtonsState();

  switch (status) {
    case startingScreen:
      if (buttons.getButtonState(1))
      {
        startEscapeRoom();
      }

    break;
    case inMap:
      escapeMap.run();

      // Time mechanic and game over mechanic
      lcd.setCursor(10, 0);
      lcd.print("time:");
      lcd.setCursor(13, 1);
      lcd.print((totalTime - (millis() - startingMillis)) / 1000);

      if (((totalTime - (millis() - startingMillis)) / 1000) < 0) {
        status = gameOverScreen;
      }
      break;
    case inGame1:
      gameOne.run();
      break;
    case inGame2:
      gameTwo.run();
      break;
    case inGame3:
      // todo: make this the running game
      // gameFour.run();
      break;
    // case inGame4:
    //   gameFour.run();
      break;
    case endingScreen:
      if (!hasEnded)
      {
        hasEnded = true;
        timeLeft = ((totalTime - (millis() - startingMillis)) / 1000);
      }
      

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You have won!");
      lcd.setCursor(0, 1);
      lcd.print("Time left:");
      lcd.print(timeLeft);
      break;
    case gameOverScreen:
      lcd.setCursor(0, 0);
      lcd.print("You have ran out");
      lcd.setCursor(0,1);
      lcd.print("of time!");
      break;
  }
}
