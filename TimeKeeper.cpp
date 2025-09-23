// TimeKeeper

#include <Arduino.h>

#include "TimeKeeper.h"

TimeKeeper::TimeKeeper(unsigned long intervalMs) : intervalMs(intervalMs), lastMillis(0), timeUp(false) {}

void TimeKeeper::begin() {
  reset();
}

void TimeKeeper::update() {
  unsigned long currentMillis = millis();
  if (!timeUp && (currentMillis - lastMillis >= intervalMs)) {
    timeUp = true;
  }
}

void TimeKeeper::reset() {
  lastMillis = millis();
  timeUp = false;
}

bool TimeKeeper::isTimeUp() const {
  return timeUp;
}
