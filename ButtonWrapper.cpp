#include "ButtonWrapper.h"

#define firstButton 1
#define secondButton 2
#define thirdButton 4
#define fourthButton 8
#define fifthButton 16
#define sixthButton 32
#define seventhButton 64
#define eighthButton 128

int buttonOnePressedOn = 0;
int buttonTwoPressedOn = 0;
int buttonThreePressedOn = 0;
int buttonFourPressedOn = 0;
int buttonFivePressedOn = 0;
int buttonSixPressedOn = 0;
int buttonSevenPressedOn = 0;
int buttonEightPressedOn = 0;

bool lastStateOne = false;
bool lastStateTwo = false;
bool lastStateThree = false;
bool lastStateFour = false;
bool lastStateFive = false;
bool lastStateSix = false;
bool lastStateSeven = false;
bool lastStateEight = false;


// ButtonWrapper class
// UpdateButtons -> reads, debounces and updates internal state
// GetButtonsState -> returns struct of pressed buttons
// GetButtonState(buttonId) -> return one specific button

ButtonWrapper::ButtonWrapper(TM1638plus &tm) : tm(tm) {
}

void ButtonWrapper::updateButtons() {
  int buttonData = tm.readButtons(); // Read the combined value of every button

  // Mask each button's state
  bool readingOne = buttonData & firstButton;
  bool readingTwo = buttonData & secondButton;
  bool readingThree = buttonData & thirdButton;
  bool readingFour = buttonData & fourthButton;
  bool readingFive = buttonData & fifthButton;
  bool readingSix = buttonData & sixthButton;
  bool readingSeven = buttonData & seventhButton;
  bool readingEight = buttonData & eighthButton;

  // Update debounce timers if state changed
  if (readingOne != lastStateOne) {
      buttonOnePressedOn = millis();
      lastStateOne = readingOne;
  }
  if (readingTwo != lastStateTwo) {
      buttonTwoPressedOn = millis();
      lastStateTwo = readingTwo;
  }
  if (readingThree != lastStateThree) {
      buttonThreePressedOn = millis();
      lastStateThree = readingThree;
  }
  if (readingFour != lastStateFour) {
      buttonFourPressedOn = millis();
      lastStateFour = readingFour;
  }
  if (readingFive != lastStateFive) {
      buttonFivePressedOn = millis();
      lastStateFive = readingFive;
  }
  if (readingSix != lastStateSix) {
      buttonSixPressedOn = millis();
      lastStateSix = readingSix;
  }
  if (readingSeven != lastStateSeven) {
      buttonSevenPressedOn = millis();
      lastStateSeven = readingSeven;
  }
  if (readingEight != lastStateEight) {
      buttonEightPressedOn = millis();
      lastStateEight = readingEight;
  }

  // Update the button states if the debounce period has passed
  if ((millis() - buttonOnePressedOn) > 30) {
      buttonState.buttonOnePressed = readingOne;
  }
  if ((millis() - buttonTwoPressedOn) > 30) {
      buttonState.buttonTwoPressed = readingTwo;
  }
  if ((millis() - buttonThreePressedOn) > 30) {
      buttonState.buttonThreePressed = readingThree;
  }
  if ((millis() - buttonFourPressedOn) > 30) {
      buttonState.buttonFourPressed = readingFour;
  }
  if ((millis() - buttonFivePressedOn) > 30) {
      buttonState.buttonFivePressed = readingFive;
  }
  if ((millis() - buttonSixPressedOn) > 30) {
      buttonState.buttonSixPressed = readingSix;
  }
  if ((millis() - buttonSevenPressedOn) > 30) {
      buttonState.buttonSevenPressed = readingSeven;
  }
  if ((millis() - buttonEightPressedOn) > 30) {
      buttonState.buttonEightPressed = readingEight;
  }

  // Button debug
  // Serial.print(lastStateOne);
  // Serial.print(" ");
  // Serial.println(buttonState.buttonOnePressed);
}

ButtonState ButtonWrapper::getButtonsState() 
{
  return buttonState;
}

bool ButtonWrapper::getButtonState(int buttonId) {
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