/*
  AppState Management Library

  A library for managing the application state, including
  display settings and sensor data.
*/

#include "AppState.h"

#include <Arduino.h>

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
