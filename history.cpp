/*
  History Data Storage Library

  Implementation of a circular buffer for storing
  and retrieving historical sensor data.
*/

#include <Arduino.h>

#include "history.h"
#include "serial.h"

History::History(uint8_t size) : size(size), minValue(0), maxValue(0) {
  values = new float[size];
}

History::~History() {
  delete[] values;
}

void History::fill(float value) {
  for (uint8_t i = 0; i < size; i++) {
    values[i] = value;
  }
  minValue = value;
  maxValue = value;
}

void History::prepend(float value) {
  float removedValue = values[size - 1];

  for (uint8_t i = size - 1; i > 0; i--) {
    values[i] = values[i - 1];
  }
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
    for (uint8_t i = 1; i < size; i++) {
      if (values[i] < minValue) minValue = values[i];
      if (values[i] > maxValue) maxValue = values[i];
    }
  }
}

float History::getValue(uint8_t index) const {
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

uint8_t History::getSize() const {
  return size;
}
