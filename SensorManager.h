/*
  Sensor Manager Library

  A library for managing sensor hardware communication
  and data acquisition.
*/

#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <Arduino.h>

class Adafruit_AHTX0;
class Adafruit_BMP280;

class SensorManager {
public:
  SensorManager(Adafruit_AHTX0& thermometer, Adafruit_BMP280& barometer);

public:
  bool begin();
  bool readSensorData(float* temperature, float* humidity, float* pressure);

private:
  Adafruit_AHTX0& thermometer;
  Adafruit_BMP280& barometer;
};

#endif
