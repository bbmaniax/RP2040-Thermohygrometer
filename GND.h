/*
  GND Pin Controller Library

  A library for managing GPIO pins used as GND
  for external components.
*/

#ifndef __GND_H__
#define __GND_H__

#include <Arduino.h>

class GND {
public:
  GND(uint8_t pin);

public:
  void begin();

private:
  uint8_t pin;
};

#endif // __GND_H__
