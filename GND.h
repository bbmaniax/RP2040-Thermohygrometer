// GND - Manages a GPIO pin configured as ground (GND).

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

#endif  // __GND_H__
