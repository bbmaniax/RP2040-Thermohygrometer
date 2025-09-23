// History

#include <Arduino.h>

#include "History.h"

History::History(int16_t* buffer, size_t size) : size(size), buffer(buffer), minValue(0), maxValue(0) {}

void History::fill(int16_t value) {
  for (size_t i = 0; i < size; i++) { buffer[i] = value; }
  minValue = value;
  maxValue = value;
}

void History::prepend(int16_t value) {
  int16_t removedValue = buffer[size - 1];

  memmove(&buffer[1], &buffer[0], (size - 1) * sizeof(int16_t));
  buffer[0] = value;

  if (value > maxValue) { maxValue = value; }
  if (value < minValue) { minValue = value; }

  if (removedValue == minValue || removedValue == maxValue) {
    minValue = buffer[0];
    maxValue = buffer[0];
    for (size_t i = 1; i < size; i++) {
      if (buffer[i] < minValue) minValue = buffer[i];
      if (buffer[i] > maxValue) maxValue = buffer[i];
    }
  }
}

int16_t History::getValue(size_t index) const {
  if (index < size) { return buffer[index]; }
  return 0;
}

int16_t History::getMaxValue() const {
  return maxValue;
}

int16_t History::getMinValue() const {
  return minValue;
}
