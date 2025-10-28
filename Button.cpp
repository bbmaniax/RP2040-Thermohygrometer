// Button

#include <Arduino.h>

#include "Button.h"

Button::Button(uint8_t pin, bool usePullUp, unsigned long debounceDelay, unsigned long longPressDelay) : pin(pin), usePullUp(usePullUp), debounceDelay(debounceDelay), longPressDelay(longPressDelay), currentState(false), lastState(false), lastReading(false), lastDebounceTime(0), pressStartTime(0), longPressTriggered(false) {}

void Button::begin() {
  pinMode(pin, usePullUp ? INPUT_PULLUP : INPUT);
  currentState = false;
  lastState = currentState;
  lastReading = currentState;
  lastDebounceTime = millis();
  pressStartTime = 0;
  longPressTriggered = false;
}

void Button::update() {
  bool reading = !digitalRead(pin);
  if (reading != lastReading) { lastDebounceTime = millis(); }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastState = currentState;
    currentState = reading;

    if (!lastState && currentState) {
      pressStartTime = millis();
      longPressTriggered = false;
    }

    if (lastState && !currentState) {
      if (!longPressTriggered && (millis() - pressStartTime) >= longPressDelay) {
        longPressTriggered = true;
      } else {
        longPressTriggered = false;
      }
      pressStartTime = 0;
    }
  }
  lastReading = reading;
}

bool Button::isPressed() {
  return currentState;
}

bool Button::isClicked() {
  return lastState && !currentState && !longPressTriggered;
}

bool Button::isLongPressed() {
  return lastState && !currentState && longPressTriggered;
}
