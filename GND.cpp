// GND - Manages a GPIO pin configured as ground (GND).

#include <Arduino.h>

#include "GND.h"

GND::GND(uint8_t pin) : pin(pin) {}

void GND::begin() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}
