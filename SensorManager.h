// SensorManager

#ifndef __SENSOR_MANAGER_H__
#define __SENSOR_MANAGER_H__

#include <Arduino.h>

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
  SensorData getSensorData();

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

#endif  // __SENSOR_MANAGER_H__
