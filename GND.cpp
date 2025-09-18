/*
  GND Pin Controller Library

  A library for managing GPIO pins used as GND
  for external components.
*/

#include "GND.h"

#include <Arduino.h>

GND::GND(uint8_t pin) : pin(pin) {
}

void GND::begin() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}
