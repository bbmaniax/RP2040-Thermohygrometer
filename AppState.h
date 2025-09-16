/*
  AppState Management Library

  A library for managing the application state, including
  display settings and sensor data.
*/

#ifndef APPSTATE_H
#define APPSTATE_H

#include <Arduino.h>

class AppState {
public:
  AppState(unsigned long sensorReadIntervalMs);

public:
  void begin();
  bool shouldReadSensorData();
  void markSensorDataAsRead();

private:
  unsigned long lastMillis;
  unsigned long sensorReadIntervalMs;
};

#endif
