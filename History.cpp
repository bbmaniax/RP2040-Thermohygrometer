/*
  History Data Storage Library

  Implementation of a circular buffer for storing
  and retrieving historical sensor data.
*/

#include "History.h"

#include <Arduino.h>

#include "DebugSerial.h"

History::History(float* buffer, size_t size) : size(size), buffer(buffer), minValue(0), maxValue(0) {}

History::~History() {}

void History::fill(float value) {
  for (size_t i = 0; i < size; i++) { buffer[i] = value; }
  minValue = value;
  maxValue = value;
}

void History::prepend(float value) {
  float removedValue = buffer[size - 1];

  memmove(&buffer[1], &buffer[0], (size - 1) * sizeof(float));
  buffer[0] = value;

  if (value > maxValue) {
    maxValue = value;
  }
  if (value < minValue) {
    minValue = value;
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
    return buffer[index];
  }
  return 0;
}

float History::getMaxValue() const {
  return maxValue;
}

float History::getMinValue() const {
  return minValue;
}

size_t History::getSize() const {
  return size;
}
