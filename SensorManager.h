// SensorManager.h - Temperature and pressure sensor manager

#pragma once

#ifndef SENSOR_MANAGER_H
#  define SENSOR_MANAGER_H

#include <Arduino.h>

#  define INVALID_SENSOR_VALUE INT16_MIN
#  define IS_VALID_SENSOR_VALUE(value) ((value) != INVALID_SENSOR_VALUE)

class Adafruit_AHTX0;
class Adafruit_BMP280;

class SensorManager {
 public:
  struct SensorData {
    int16_t temperature;
    int16_t humidity;
    int16_t pressure;
  };

  SensorManager(Adafruit_AHTX0& thermometer, Adafruit_BMP280& barometer, unsigned long intervalMs = 3000);

  void begin();
  void update();

  bool isReady() const;
  bool consumeReady();
  SensorData getSensorData() const;

 private:
  enum State { IDLE, READING };

  Adafruit_AHTX0& thermometer;
  Adafruit_BMP280& barometer;
  State state;
  unsigned long lastReadTime;
  unsigned long interval;
  SensorData lastData;
  bool resultReady;
};

#endif  // SENSOR_MANAGER_H
