// Button

#include <Arduino.h>

#include "Button.h"

Button::Button(uint8_t pin, unsigned long debounceDelay) : pin(pin), debounceDelay(debounceDelay), currentState(false), lastState(false), lastReading(false), lastDebounceTime(0) {}

void Button::begin() {
  pinMode(pin, INPUT_PULLUP);
  currentState = false;
  lastState = currentState;
  lastReading = currentState;
  lastDebounceTime = millis();
}

void Button::update() {
  bool reading = !digitalRead(pin);
  if (reading != lastReading) { lastDebounceTime = millis(); }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastState = currentState;
    currentState = reading;
  }
  lastReading = reading;
}

bool Button::isPressed() {
  return currentState;
}

bool Button::isClicked() {
  return lastState && !currentState;
}
