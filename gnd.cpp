/*
  GND Pin Controller Library

  Implementation for managing GPIO pins used as GND
  for external components.
*/

#include <Arduino.h>

#include "gnd.h"
#include "serial.h"

GND::GND(uint8_t pin) : pin(pin) {
}

void GND::begin() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

uint8_t GND::getPin() const {
  return pin;
}
