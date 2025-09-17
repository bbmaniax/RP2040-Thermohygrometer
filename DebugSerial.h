/*
  DebugSerial Output Library

  A library for debug serial communication.
*/

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <Arduino.h>

#if defined(DEBUG)
#define SERIAL_BEGIN(x) Serial.begin(x)
#define WAIT_FOR_SERIAL() \
  while (!Serial) { delay(100); }
#define SERIAL_PRINT(x) Serial.print(x)
#define SERIAL_PRINTLN(x) Serial.println(x)
#define SERIAL_PRINTHEX(x) Serial.print(x, HEX)
#define SERIAL_PRINTLNHEX(x) Serial.println(x, HEX)
#else
#define SERIAL_BEGIN(x)
#define WAIT_FOR_SERIAL()
#define SERIAL_PRINT(x)
#define SERIAL_PRINTLN(x)
#define SERIAL_PRINTHEX(x)
#define SERIAL_PRINTLNHEX(x)
#endif

#endif // __SERIAL_H__
