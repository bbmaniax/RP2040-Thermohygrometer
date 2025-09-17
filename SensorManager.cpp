/*
  SensorManager Library

  A library for managing sensor hardware communication
  and data acquisition.
*/

#include "SensorManager.h"

#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

#include "DebugSerial.h"

SensorManager::SensorManager(Adafruit_AHTX0& thermometer, Adafruit_BMP280& barometer)
  : thermometer(thermometer), barometer(barometer) {}

bool SensorManager::begin() {
  if (!thermometer.begin()) {
    SERIAL_PRINTLN("Failed to initialize AHTX0!");
    return false;
  }
  if (!barometer.begin()) {
    SERIAL_PRINTLN("Failed to initialize BMP280!");
    return false;
  }
  return true;
}

bool SensorManager::readSensorData(SensorValues* values) {
  sensors_event_t temperatureEvent, humidityEvent;
  thermometer.getEvent(&humidityEvent, &temperatureEvent);
  if (isnan(temperatureEvent.temperature) || isnan(humidityEvent.relative_humidity)) {
    SERIAL_PRINTLN("Failed to read AHTX0!");
    return false;
  }
  values->temperature = (int16_t)(temperatureEvent.temperature * 10);
  values->humidity = (int16_t)(humidityEvent.relative_humidity * 10);

  float pressure = barometer.readPressure();
  if (isnan(pressure)) {
    SERIAL_PRINTLN("Failed to read BMP280!");
    return false;
  }
  values->pressure = (int16_t)(pressure / 10.0f);

  return true;
}
