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
  void fill(int16_t value);
  void prepend(int16_t value);
  int16_t getValue(size_t index) const;
  int16_t getMaxValue() const;
  int16_t getMinValue() const;

private:
  size_t size;
  int16_t* buffer;
  int16_t minValue;
  int16_t maxValue;
};

#endif // __HISTORY_H__
