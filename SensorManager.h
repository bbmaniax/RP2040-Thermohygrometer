/*
  SensorManager Library

  A library for managing sensor hardware communication
  and data acquisition.
*/

#ifndef __SENSOR_MANAGER_H__
#define __SENSOR_MANAGER_H__

#include <Arduino.h>

class Adafruit_AHTX0;
class Adafruit_BMP280;

struct SensorValues {
  int16_t temperature;
  int16_t humidity;
  int16_t pressure;
};

class SensorManager {
public:
  SensorManager(Adafruit_AHTX0& thermometer, Adafruit_BMP280& barometer);

public:
  bool begin();
  bool readSensorData(SensorValues* values);

private:
  Adafruit_AHTX0& thermometer;
  Adafruit_BMP280& barometer;
};

#endif // __SENSOR_MANAGER_H__
