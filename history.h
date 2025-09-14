/*
  History Data Storage Library

  A circular buffer for storing and retrieving
  historical sensor data.
*/

#ifndef __HISTORY_H__
#define __HISTORY_H__

#include <Arduino.h>

class History {
public:
  History(uint8_t size);
  ~History();
  void fill(float value);
  void prepend(float value);
  float getValue(uint8_t index) const;
  float getMaxValue() const;
  float getMinValue() const;
  uint8_t getSize() const;

private:
  uint8_t size;
  float* values;
  float minValue;
  float maxValue;
};

#endif // __HISTORY_H__
