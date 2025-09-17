/*
 AppViewState Management Library

 A library for managing the view state, including
 display patterns and orientation.
*/

#include "AppViewState.h"

#include <Arduino.h>

AppViewState::AppViewState() : patternIndex(0), displayFlipped(false) {}

void AppViewState::begin() {
  patternIndex = 0;
  displayFlipped = false;
}

uint8_t AppViewState::getPatternIndex() const {
  return patternIndex;
}

void AppViewState::switchToNextPattern() {
  patternIndex = (patternIndex + 1) % 4;
}

bool AppViewState::isDisplayFlipped() const {
  return displayFlipped;
}

void AppViewState::flipDisplay() {
  displayFlipped = !displayFlipped;
}
