/*
  History Data Storage Library

  Implementation of a circular buffer for storing
  and retrieving historical sensor data.
*/

#include "History.h"

#include <Arduino.h>

#include "DebugSerial.h"

History::History(size_t size) : size(size), minValue(0), maxValue(0) {
  values = new float[size];
}

History::~History() {
  delete[] values;
}

void History::fill(float value) {
  for (size_t i = 0; i < size; i++) { values[i] = value; }
  minValue = value;
  maxValue = value;
}

void History::prepend(float value) {
  float removedValue = values[size - 1];

  memmove(&values[1], &values[0], (size - 1) * sizeof(float));
  values[0] = value;

  if (value > maxValue) {
    maxValue = value;
  }
  if (value < minValue) {
    minValue = value;
  }

  if (removedValue == minValue || removedValue == maxValue) {
    minValue = values[0];
    maxValue = values[0];
    for (size_t i = 1; i < size; i++) {
      if (values[i] < minValue) minValue = values[i];
      if (values[i] > maxValue) maxValue = values[i];
    }
  }
}

float History::getValue(size_t index) const {
  if (index < size) {
    return values[index];
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
