// Button

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>

class Button {
public:
  Button(uint8_t pin, unsigned long debounceDelay = 20, unsigned long longPressDelay = 1000);

  void begin();
  void update();

  bool isPressed();
  bool isClicked();
  bool isLongPressed();

private:
  uint8_t pin;
  unsigned long debounceDelay;
  unsigned long longPressDelay;

  bool currentState;
  bool lastState;
  bool lastReading;
  unsigned long lastDebounceTime;
  unsigned long pressStartTime;
  bool longPressTriggered;
};

#endif  // __BUTTON_H__
