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
  History(float* buffer, size_t size);
  ~History();
  void fill(float value);
  void prepend(float value);
  float getValue(size_t index) const;
  float getMaxValue() const;
  float getMinValue() const;
  size_t getSize() const;

private:
  size_t size;
  float* buffer;
  float minValue;
  float maxValue;
};

#endif // __HISTORY_H__
