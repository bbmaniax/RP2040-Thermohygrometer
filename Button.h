// Button

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>

class Button {
public:
  Button(uint8_t pin, unsigned long debounceDelay = 20);

  void begin();
  void update();

  bool isPressed();
  bool isClicked();

private:
  uint8_t pin;
  unsigned long debounceDelay;

  bool currentState;
  bool lastState;
  bool lastReading;
  unsigned long lastDebounceTime;
};

#endif  // __BUTTON_H__
