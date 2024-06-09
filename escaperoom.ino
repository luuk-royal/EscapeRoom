#include <TM1638plus.h>
#include <LiquidCrystal_I2C.h>
#include "EscapeRoomStatus.h"
#include "ButtonWrapper.h"
#include "Music.h"
#include "EscapeMap.h"
#include "BalanceGame.h"
#include "WhackAMole.h"

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
bool started = false;

GamesDone gamesDone;

LiquidCrystal_I2C lcd(0x27, 16, 2);
TM1638plus tm(STB, CLK, DIO);
ButtonWrapper buttons(tm);
Music music(BUZZER);
EscapeMap escapeMap(lcd, buttons, status, gamesDone);
BalanceGame gameOne(POTMETER, LED_RRR, LED_RR, LED_GR, LED_YC, LED_GL, LED_RL, LED_RLL, status, gamesDone, lcd);
WhackAMole gameTwo(tm, buttons, status, gamesDone);

void startEscapeRoom() {
  Serial.println("In startEscapeRoom!");
  status = inMap;
  escapeMap.setup();

  // Run the updateMap manually as the map itself only updates on movement.
  escapeMap.updateMap();
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
  
  // Setup game one
  gameOne.setUp();
}

void loop() {
  buttons.updateButtons();
  // music.PlayMusic();


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
      break;
    case inGame1:
      gameOne.run();
      break;
    case inGame2:
      gameTwo.run();
      break;
    case inGame3:
      status = inMap;
      break;
    case inGame4:
      status = inMap;
      break;
  }
}
