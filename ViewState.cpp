/*
 ViewState Management Library

 A library for managing the view state, including
 display patterns and orientation.
*/

#include "ViewState.h"

#include <Arduino.h>

ViewState::ViewState() : patternIndex(0), displayFlipped(false) {}

void ViewState::begin() {
  patternIndex = 0;
  displayFlipped = false;
}

uint8_t ViewState::getPatternIndex() const {
  return patternIndex;
}

void ViewState::switchToNextPattern() {
  patternIndex = (patternIndex + 1) % 4;
}

bool ViewState::isDisplayFlipped() const {
  return displayFlipped;
}

void ViewState::flipDisplay() {
  displayFlipped = !displayFlipped;
}
