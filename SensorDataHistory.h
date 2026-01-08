// SensorDataHistory.h - Sensor data history buffer

#pragma once

#ifndef SENSOR_DATA_HISTORY_H
#  define SENSOR_DATA_HISTORY_H

#  include <Arduino.h>

#  include "SensorManager.h"

class SensorDataHistory {
 public:
  SensorDataHistory(int16_t* buffer, size_t size);

  void begin();
  void prepend(int16_t value);

  size_t getCount() const;
  int16_t getValue(size_t index) const;
  void getMinMaxValue(size_t count, int16_t& minValue, int16_t& maxValue) const;

 private:
  int16_t* buffer;
  size_t size;
  size_t count;
};

#endif  // SENSOR_DATA_HISTORY_H
