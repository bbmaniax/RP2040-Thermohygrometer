// TimeKeeper - A simple time interval manager

#include <Arduino.h>

#include "TimeKeeper.h"

TimeKeeper::TimeKeeper(unsigned long intervalMs) : intervalMs(intervalMs), lastMillis(0), timeUp(false) {}

void TimeKeeper::begin() {
  lastMillis = 0;
  timeUp = false;
}

void TimeKeeper::update() {
  unsigned long currentMillis = millis();
  if (!timeUp && (currentMillis - lastMillis >= intervalMs)) {
    timeUp = true;
  }
}

bool TimeKeeper::isTimeUp() const {
  return timeUp;
}

void TimeKeeper::reset() {
  lastMillis = millis();
  timeUp = false;
}
