#include <TM1638plus.h>
#include <LiquidCrystal_I2C.h>
#include <pitches.h>

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

int lastMilis = 0;

// main soundtrack
int melody[] = { NOTE_A4, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_G5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_G5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_C5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4};
int noteLength[] = {250,  375,     1000,    125,     125,     250,     375,     500,     125,     125,     250,     750,     125,     125,     125,     750,     500,     250,     250,     250,     250,     250,     250,     750,     500,     500,     500,     250,     250,     500,     125,     125,     250,     750,     500,     500,     500,     250,     1250,    250,    250};
int length = sizeof(melody) / sizeof(melody[0]);

// Make the music timing work;
int toneTimeStamp = 0;
int toneDuration = 0;

// Keep track of the current tone
int currentToneIndex = 0;

enum escapeRoomStatus {
  inMap = 0,
  inGame1 = 1,
  inGame2 = 2,
  inGame3 = 3,
  inGame4 = 4,
  startingScreen = 5,
  endingScreen = 6
};

byte status = 0;
bool started = false;

bool enabledButtons[] = {
  true,
  false,
  false,
  false,
  false,
  false,
  false,
  false
};

void startEscapeRoom() {
  status = 0;
}

// The idea for making the functions the buttons call modular came from here:
// https://codereview.stackexchange.com/questions/210356/improvement-for-stdmap-int-run-some-function-double-dispatch
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
  // lcd.createChar(0, menuBlock);
  // lcd.setCursor(0, 0);
  // lcd.write(0);
  // delay(1000);
  // lcd.createChar(0, customChar);
  // lcd.setCursor(0, 1);
  // lcd.write(0);
}

void loop() {
  readTMButtons (enabledButtons, buttonFunctions);
  // put your main code here, to run repeatedly:

  // music();

  // Serial.println((int) tm.readButtons());
  // delay(500);

  // tm.setLED(0, 1);

  switch (status) {
    case startingScreen:
    // Do nothing
    case inMap:
    Serial.println("inMap");
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

void music(){

  if (millis() - toneTimeStamp > toneDuration) {
    toneTimeStamp = millis();
    toneDuration = noteLength[currentToneIndex];
    tone(BUZZER, melody[currentToneIndex], toneDuration);

    if (currentToneIndex >= (length - 1) ) {
      currentToneIndex = 0;
    } else {
      currentToneIndex++;
    }
  }

  // if (millis() - toneTimeStamp > toneDuration) {
  //   toneTimeStamp = millis();
  //   switch (currentTone){
  //     case 0:
  //       toneDuration = 400;
  //       tone(BUZZER, NOTE_B0, toneDuration);
  //       currentTone++;
  //       break;
  //     case 1:
  //       toneDuration = 500;
  //       tone(BUZZER, NOTE_B1, toneDuration);
  //       currentTone++;
  //       break;
  //     case 2:
  //       toneDuration = 200;
  //       tone(BUZZER, NOTE_GS3, toneDuration);
  //       currentTone++;
  //       break;
  //     case 3:
  //       toneDuration = 300;
  //       tone(BUZZER, NOTE_G3, toneDuration);
  //       currentTone = 0;
  //       break;
  //   }
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

// Keep track of the player
int playerMap[][10] = {
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,2,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
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

  // Run through the left side of the map to check for player position and for special rooms
  for(int i=0; i < 8; i++) {
    for(int j = 0; j < 5; j++) {
      // Once the player has been found exit out of this function
      if (playerMap[i][j] == 2) {
        byte temp = 0B1;
        temp << j;
        temp += 1;
        temp << (5-j);

        tempLeftMap[i] = temp;

        // reassing the map
        displayUpdate(tempLeftMap, tempRightMap);

        return;
      }
    } 
  }

  // Run through the left side of the map to check for player position and for special rooms
  for(int i=0; i < 8; i++) {
    for(int j = 0; j < 5; j++) {
      // Once the player has been found exit out of this function
      if (playerMap[i][j] == 2) {
        byte temp = 1;
        temp << j;
        temp += 1;
        temp << (5-j);

        tempRightMap[i] = temp;

        // reassing the map
        displayUpdate(tempLeftMap, tempRightMap);

        return;
      }
    } 
  }
}

void mapSetup() {
  lcd.createChar(0, leftMap);
  lcd.createChar(1, rightMap);
  lcd.setCursor(0, 0);
  lcd.write(0);
  lcd.write(1);
}

// replace the characters in the memory and switch back to the proper memory
void displayUpdate(byte left[], byte right[]) {
  lcd.createChar(0, left);
  lcd.createChar(1, right);
  lcd.setCursor(0, 0);
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

// Bind enabled buttons and their respective functions to the buttons
void readTMButtons(bool enabled[], std::function<void()> functions[]) {
  int buttonData = tm.readButtons(); // Lees de gecombineerde waarde van alle ingedrukte knoppen

  // Controleer elke knop afzonderlijk
  if (buttonData & firstButton & enabled[0]) {
    // Eerste knop is ingedrukt
  }
  if (buttonData & secondButton & enabled[1]) {
    // Tweede knop is ingedrukt
  }
  if (buttonData & thirdButton & enabled[2]) {
    // Derde knop is ingedrukt
  }
  if (buttonData & fourthButton & enabled[3]) {
    // Derde knop is ingedrukt
  }
  if (buttonData & fifthButton & enabled[4]) {
    // Derde knop is ingedrukt
  }
  if (buttonData & sixthButton & enabled[5]) {
    // Derde knop is ingedrukt
  }
  if (buttonData & seventhButton & enabled[6]) {
    // Derde knop is ingedrukt
  }
  if (buttonData & eigthButton & enabled[7]) {
    // Derde knop is ingedrukt
  }

}

