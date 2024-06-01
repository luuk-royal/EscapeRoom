#include "Music.h"
#include <pitches.h>

// main soundtrack
int melodyOne[] = { NOTE_A4, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_G5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4, NOTE_A5, NOTE_G5, NOTE_E5, NOTE_G5, NOTE_C5, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_C5, NOTE_C5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_G4};
int noteLengthTrackOne[] = {250,  375,     1000,    125,     125,     250,     375,     500,     125,     125,     250,     750,     125,     125,     125,     750,     500,     250,     250,     250,     250,     250,     250,     750,     500,     500,     500,     250,     250,     500,     125,     125,     250,     750,     500,     500,     500,     250,     1250,    250,    250};
int lengthOne = sizeof(melodyOne) / sizeof(melodyOne[0]);

// Make the music timing work;
int toneTimeStamp = 0;
int toneDuration = 0;

// Keep track of the current tone
int currentToneIndex = 0;

void Music::PlayMusic()
{
    if (millis() - toneTimeStamp > toneDuration) {
        switch (expression)
        {
        default: // Put the default at the first track since the first track is the default
        case 1:
            toneTimeStamp = millis();
            toneDuration = noteLengthTrackOne[currentToneIndex];
            tone(BUZZER, melodyOne[currentToneIndex], toneDuration);

            if (currentToneIndex >= (lengthOne - 1) ) {
            currentToneIndex = 0;
            } else {
            currentToneIndex++;
            }
            
            break;
        case 2:
            // TODO:
            // Change this to a second theme
            toneTimeStamp = millis();
            toneDuration = noteLengthTrackOne[currentToneIndex];
            tone(BUZZER, melodyOne[currentToneIndex], toneDuration);

            if (currentToneIndex >= (lengthOne - 1) ) {
            currentToneIndex = 0;
            } else {
            currentToneIndex++;
            }

            break;
        case 3:
            // TODO:
            // Change this to the final boss theme
            toneTimeStamp = millis();
            toneDuration = noteLengthTrackOne[currentToneIndex];
            tone(BUZZER, melodyOne[currentToneIndex], toneDuration);

            if (currentToneIndex >= (lengthOne - 1) ) {
            currentToneIndex = 0;
            } else {
            currentToneIndex++;
            }
            
            break;
        }
    }
}

void Music::SwitchTrack(int trackId)
{
    currentTrack = trackId;
    ResetMusic();
}

void Music::ResetMusic()
{
    currentToneIndex = 0;
}


void music(){

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
