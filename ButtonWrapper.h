#pragma once
#ifndef BUTTON_WRAPPER_H
#define BUTTON_WRAPPER_H

#include <Arduino.h>
#include <TM1638plus.h>

// ButtonWrapper class
// UpdateButtons -> reads, debounces and updates internal state
// GetButtonsState -> returns struct of pressed buttons
// GetButtonState(buttonId) -> return one specific button

struct ButtonState {
  bool buttonOnePressed = false;
  bool buttonTwoPressed = false;
  bool buttonThreePressed = false;
  bool buttonFourPressed = false;
  bool buttonFivePressed = false;
  bool buttonSixPressed = false;
  bool buttonSevenPressed = false;
  bool buttonEightPressed = false;
};

class ButtonWrapper
{
public:
  ButtonWrapper(TM1638plus &tm);
  void updateButtons();
  struct ButtonState getButtonsState();
  bool getButtonState(int buttonId);
private:
  TM1638plus &tm; // Reference to the TM1638plus object
};

#endif // BUTTON_WRAPPER_H