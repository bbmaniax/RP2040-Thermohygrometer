/*
  GND Pin Controller Library

  Implementation for managing GPIO pins used as GND
  for external components.
*/

#include "GND.h"

#include <Arduino.h>

#include "DebugSerial.h"

GND::GND(uint8_t pin) : pin(pin) {
}

void GND::begin() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

uint8_t GND::getPin() const {
  return pin;
}
