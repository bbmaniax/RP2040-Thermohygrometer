/*
  Debug Serial Library

  Serial communication handling for debugging purposes.
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
#else
#define SERIAL_BEGIN(x)
#define WAIT_FOR_SERIAL()
#define SERIAL_PRINT(x)
#define SERIAL_PRINTLN(x)
#endif

#endif // __SERIAL_H__
