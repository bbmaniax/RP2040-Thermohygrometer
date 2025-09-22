// SensorManager - Manages sensor initialization and data retrieval.

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Arduino.h>

#include "DebugSerial.h"
#include "SensorManager.h"

SensorManager::SensorManager(Adafruit_AHTX0& thermometer, Adafruit_BMP280& barometer) : thermometer(thermometer), barometer(barometer) {}

void SensorManager::begin() {
  DEBUG_SERIAL_PRINTLN("Initializing SensorManager");
  if (!thermometer.begin()) { DEBUG_SERIAL_PRINTLN("Failed to initialize AHTX0!"); }
  if (!barometer.begin()) { DEBUG_SERIAL_PRINTLN("Failed to initialize BMP280!"); }
}

void SensorManager::readSensorData(SensorValues* values) {
  // DEBUG_SERIAL_PRINTLN("Reading sensor data");
  sensors_event_t temperatureEvent, humidityEvent;
  if (thermometer.getEvent(&humidityEvent, &temperatureEvent)) {
    values->temperature = static_cast<int16_t>(temperatureEvent.temperature * 10.0f);
    values->humidity = static_cast<int16_t>(humidityEvent.relative_humidity * 10.0f);
  } else {
    DEBUG_SERIAL_PRINTLN("Failed to read AHTX0!");
  }
  float pressure = barometer.readPressure();
  if (!isnan(pressure)) {
    values->pressure = static_cast<int16_t>(pressure / 10.0f);
  } else {
    DEBUG_SERIAL_PRINTLN("Failed to read BMP280!");
  }
}
