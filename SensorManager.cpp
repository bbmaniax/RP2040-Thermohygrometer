// SensorManager.cpp - Temperature and pressure sensor manager

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Arduino.h>

#include "DebugSerial.h"
#include "SensorManager.h"

SensorManager::SensorManager(Adafruit_AHTX0& thermometer, Adafruit_BMP280& barometer, unsigned long intervalMs)
    : thermometer(thermometer), barometer(barometer), state(IDLE), lastReadTime(0), resultReady(false) {
  interval = intervalMs;
  lastData.temperature = 0;
  lastData.humidity = 0;
  lastData.pressure = 0;
}

void SensorManager::begin() {
  // DEBUG_SERIAL_PRINTLN("Initializing SensorManager");
  if (!thermometer.begin()) { DEBUG_SERIAL_PRINTLN("Failed to initialize AHTX0!"); }
  if (!barometer.begin()) { DEBUG_SERIAL_PRINTLN("Failed to initialize BMP280!"); }
  state = IDLE;
  lastReadTime = millis() - interval;
  resultReady = false;
}

void SensorManager::update() {
  switch (state) {
    case IDLE:
      if (millis() - lastReadTime >= interval) {
        state = READING;
      }
      break;

    case READING: {
      // DEBUG_SERIAL_PRINTLN("Reading sensor data");
      sensors_event_t temperatureEvent, humidityEvent;
      if (thermometer.getEvent(&humidityEvent, &temperatureEvent)) {
        lastData.temperature = static_cast<int16_t>(temperatureEvent.temperature * 10.0f);
        lastData.humidity = static_cast<int16_t>(humidityEvent.relative_humidity * 10.0f);
      } else {
        DEBUG_SERIAL_PRINTLN("Failed to read AHTX0!");
      }
      float pressure = barometer.readPressure();
      if (!isnan(pressure)) {
        lastData.pressure = static_cast<int16_t>(pressure / 10.0f);
      } else {
        DEBUG_SERIAL_PRINTLN("Failed to read BMP280!");
      }
      resultReady = true;
      lastReadTime = millis();
      state = IDLE;
    } break;
  }
}

bool SensorManager::isReady() const {
  return resultReady;
}

SensorManager::SensorData SensorManager::getSensorData() {
  resultReady = false;
  return lastData;
}
