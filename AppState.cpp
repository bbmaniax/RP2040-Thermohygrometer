// AppState - Manages the application state, including timing for sensor reads.

#include <Arduino.h>

#include "AppState.h"

AppState::AppState(unsigned long sensorReadIntervalMs) : lastMillis(0), sensorReadIntervalMs(sensorReadIntervalMs) {}

void AppState::begin() {
  lastMillis = 0;
}

bool AppState::shouldReadSensorData() {
  unsigned long currentMillis = millis();
  return (lastMillis == 0 || currentMillis - lastMillis >= sensorReadIntervalMs);
}

void AppState::markSensorDataAsRead() {
  lastMillis = millis();
}
