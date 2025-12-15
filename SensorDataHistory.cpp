// SensorDataHistory.cpp - Sensor data history buffer

#include <Arduino.h>

#include "SensorDataHistory.h"

SensorDataHistory::SensorDataHistory(int16_t* buffer, size_t size) : buffer(buffer), size(size), count(0) {}

void SensorDataHistory::begin() {
  count = 0;
}

void SensorDataHistory::prepend(int16_t value) {
  for (size_t i = (count == size ? size - 1 : count); i > 0; i--) {
    buffer[i] = buffer[i - 1];
  }
  buffer[0] = value;
  if (count < size) count++;
}

size_t SensorDataHistory::getCount() const {
  return count;
}

int16_t SensorDataHistory::getValue(size_t index) const {
  if (index < count) {
    return buffer[index];
  }
  return INVALID_SENSOR_VALUE;
}

void SensorDataHistory::getMinMaxValue(size_t count, int16_t& minValue, int16_t& maxValue) const {
  minValue = INVALID_SENSOR_VALUE;
  maxValue = INVALID_SENSOR_VALUE;
  bool foundValid = false;

  size_t checkCount = (this->count < count) ? this->count : count;

  for (size_t i = 0; i < checkCount; i++) {
    if (IS_VALID_SENSOR_VALUE(buffer[i])) {
      if (!foundValid) {
        minValue = buffer[i];
        maxValue = buffer[i];
        foundValid = true;
      } else {
        if (buffer[i] < minValue) minValue = buffer[i];
        if (buffer[i] > maxValue) maxValue = buffer[i];
      }
    }
  }
}
