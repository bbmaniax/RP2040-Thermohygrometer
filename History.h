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
  History(int16_t* buffer, size_t size);
  ~History();
  void fill(float value);
  void prepend(float value);
  float getValue(size_t index) const;
  float getMaxValue() const;
  float getMinValue() const;
  size_t getSize() const;

private:
  size_t size;
  int16_t* buffer;
  int16_t minValue;
  int16_t maxValue;
};

#endif // __HISTORY_H__
