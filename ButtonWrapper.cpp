#include "ButtonWrapper.h"
#include <TM1638plus.h>

#define firstButton 1
#define secondButton 2
#define thirdButton 4
#define fourthButton 8
#define fifthButton 16
#define sixthButton 32
#define seventhButton 64
#define eighthButton 128

struct ButtonState buttonState;

int buttonOnePressedOn = 0;
int buttonTwoPressedOn = 0;
int buttonThreePressedOn = 0;
int buttonFourPressedOn = 0;
int buttonFivePressedOn = 0;
int buttonSixPressedOn = 0;
int buttonSevenPressedOn = 0;
int buttonEightPressedOn = 0;

int lastStateOne;
int lastStateTwo;
int lastStateThree;
int lastStateFour;
int lastStateFive;
int lastStateSix;
int lastStateSeven;
int lastStateEight;


// ButtonWrapper class
// UpdateButtons -> reads, debounces and updates internal state
// GetButtonsState -> returns struct of pressed buttons
// GetButtonState(buttonId) -> return one specific button

ButtonWrapper::ButtonWrapper(TM1638plus &tm) : tm(tm) {
}

void ButtonWrapper::updateButtons() {
    int buttonData = tm.readButtons(); // Read the combined value of every button

    // Mask each button's state
    int readingOne = buttonData & firstButton;
    int readingTwo = buttonData & secondButton;
    int readingThree = buttonData & thirdButton;
    int readingFour = buttonData & fourthButton;
    int readingFive = buttonData & fifthButton;
    int readingSix = buttonData & sixthButton;
    int readingSeven = buttonData & seventhButton;
    int readingEight = buttonData & eighthButton;

    // Update debounce timers if state changed
    if (readingOne != lastStateOne) {
        buttonOnePressedOn = millis();
    }
    if (readingTwo != lastStateTwo) {
        buttonTwoPressedOn = millis();
    }
    if (readingThree != lastStateThree) {
        buttonThreePressedOn = millis();
    }
    if (readingFour != lastStateFour) {
        buttonFourPressedOn = millis();
    }
    if (readingFive != lastStateFive) {
        buttonFivePressedOn = millis();
    }
    if (readingSix != lastStateSix) {
        buttonSixPressedOn = millis();
    }
    if (readingSeven != lastStateSeven) {
        buttonSevenPressedOn = millis();
    }
    if (readingEight != lastStateEight) {
        buttonEightPressedOn = millis();
    }

    // Update the button states if the debounce period has passed
    if ((millis() - buttonOnePressedOn) > 30) {
        buttonState.buttonOnePressed = readingOne;
        lastStateOne = readingOne;
    }
    if ((millis() - buttonTwoPressedOn) > 30) {
        buttonState.buttonTwoPressed = readingTwo;
        lastStateTwo = readingTwo;
    }
    if ((millis() - buttonThreePressedOn) > 30) {
        buttonState.buttonThreePressed = readingThree;
        lastStateThree = readingThree;
    }
    if ((millis() - buttonFourPressedOn) > 30) {
        buttonState.buttonFourPressed = readingFour;
        lastStateFour = readingFour;
    }
    if ((millis() - buttonFivePressedOn) > 30) {
        buttonState.buttonFivePressed = readingFive;
        lastStateFive = readingFive;
    }
    if ((millis() - buttonSixPressedOn) > 30) {
        buttonState.buttonSixPressed = readingSix;
        lastStateSix = readingSix;
    }
    if ((millis() - buttonSevenPressedOn) > 30) {
        buttonState.buttonSevenPressed = readingSeven;
        lastStateSeven = readingSeven;
    }
    if ((millis() - buttonEightPressedOn) > 30) {
        buttonState.buttonEightPressed = readingEight;
        lastStateEight = readingEight;
    }
}

ButtonState ButtonWrapper::getButtonsState() 
{
  return buttonState;
}

bool isCompleted(int buttonId) {
  switch(buttonId) {
      case 1: return buttonState.buttonOnePressed;
      case 2: return buttonState.buttonTwoPressed;
      case 3: return buttonState.buttonThreePressed;
      case 4: return buttonState.buttonFourPressed;
      case 5: return buttonState.buttonFivePressed;
      case 6: return buttonState.buttonSixPressed;
      case 7: return buttonState.buttonSevenPressed;
      case 8: return buttonState.buttonEightPressed;
      default: return false;
  }
}