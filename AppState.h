/*
  AppState Management Library

  A library for managing the application state, including
  display settings and sensor data.
*/

#ifndef __APP_STATE_H__
#define __APP_STATE_H__

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

#endif // __APP_STATE_H__
