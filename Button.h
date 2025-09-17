/*
  Button Input Library

  A library for handling button inputs with debouncing
  and click detection.
*/

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>

class Button {
public:
  Button(uint8_t pin, unsigned long debounceDelay = 20);

public:
  void begin();
  void update();
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
