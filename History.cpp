/*
  History Data Storage Library

  Implementation of a circular buffer for storing
  and retrieving historical sensor data.
*/

#include "History.h"

#include <Arduino.h>

#include "DebugSerial.h"

History::History(int16_t* buffer, size_t size) : size(size), buffer(buffer), minValue(0), maxValue(0) {}

History::~History() {}

void History::fill(float value) {
  int16_t intValue = (int16_t)(value * 10);
  for (size_t i = 0; i < size; i++) { buffer[i] = intValue; }
  minValue = intValue;
  maxValue = intValue;
}

void History::prepend(float value) {
  int16_t intValue = (int16_t)(value * 10);
  int16_t removedValue = buffer[size - 1];

  memmove(&buffer[1], &buffer[0], (size - 1) * sizeof(int16_t));
  buffer[0] = intValue;

  if (intValue > maxValue) {
    maxValue = intValue;
  }
  if (intValue < minValue) {
    minValue = intValue;
  }

  if (removedValue == minValue || removedValue == maxValue) {
    minValue = buffer[0];
    maxValue = buffer[0];
    for (size_t i = 1; i < size; i++) {
      if (buffer[i] < minValue) minValue = buffer[i];
      if (buffer[i] > maxValue) maxValue = buffer[i];
    }
  }
}

float History::getValue(size_t index) const {
  if (index < size) {
    return buffer[index] / 10.0f;
  }
  return 0;
}

float History::getMaxValue() const {
  return maxValue / 10.0f;
}

float History::getMinValue() const {
  return minValue / 10.0f;
}

size_t History::getSize() const {
  return size;
}
