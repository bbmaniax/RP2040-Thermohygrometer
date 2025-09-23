// GND

#ifndef __GND_H__
#define __GND_H__

#include <Arduino.h>

class GND {
public:
  GND(uint8_t pin);

  void begin();

private:
  uint8_t pin;
};

#endif  // __GND_H__
