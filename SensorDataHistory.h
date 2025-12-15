// SensorDataHistory.h - Sensor data history buffer

#pragma once

#ifndef SENSOR_DATA_HISTORY_H
#  define SENSOR_DATA_HISTORY_H

#include <Arduino.h>

#define INVALID_SENSOR_VALUE INT16_MIN
#define IS_VALID_SENSOR_VALUE(value) ((value) != INVALID_SENSOR_VALUE)

class SensorDataHistory {
public:
  SensorDataHistory(int16_t* buffer, size_t size);

  size_t getCount() const;

  void begin();
  void prepend(int16_t value);
  int16_t getValue(size_t index) const;
  void getMinMaxValue(size_t count, int16_t& minValue, int16_t& maxValue) const;

private:
  int16_t* buffer;
  size_t size;
  size_t count;
};

#endif  // SENSOR_DATA_HISTORY_H
