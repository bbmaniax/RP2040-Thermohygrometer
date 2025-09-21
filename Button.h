// Button - Handles button input with debouncing.

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>

class Button {
public:
  Button(uint8_t pin, unsigned long debounceDelay = 20);

  bool begin();
  bool update();

  bool isPressed();
  bool isClicked();

private:
  uint8_t pin;
  bool currentState;
  bool lastState;
  bool lastReading;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay;
};

#endif  // __BUTTON_H__
